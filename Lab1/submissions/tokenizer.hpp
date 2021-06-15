#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <istream>
#include <vector> 
#include <regex>
#include <map>
#include <set>
#include <iomanip>
using namespace std;

bool isNumber(const string& str)
{
    for (char const &c : str) 
    {
        if (std::isdigit(c) == 0) 
        {
            return false;
        }
    }
    return true;
}

static bool isSymbol(const string& token) 
{
    //chekc symbol patern
    std::string pattern{"[a-zA-Z][0-9a-zA-Z]*"}; 
	std::regex re(pattern);
    bool isSym = std::regex_match(token, re);
    //check symbol length
    bool checkSymLen = (token.length()< 16);
    if(!isSym)
    {
        throw invalid_argument("SYM_EXPECTED");
    }
    if(!checkSymLen)
    {
        throw invalid_argument("SYM_TOO_LONG");
    }
    return isSym && checkSymLen;
}

bool isIEAR(const string& token) 
{
    std::string pattern{"[IEAR]"}; 
	std::regex re(pattern);
    bool isIEAR = std::regex_match(token, re);
    return isIEAR;
}

class Tokenizer 
{
    public:
        void get_tokens(const std::string delim);
        bool getToken();
        void parse_tokens(int parse_mode);
        void parse_tokens_pass2(int parse_mode);
        void handle_parse_error(invalid_argument& e);
        string module_error_info = "";

        bool eof(ifstream& f_in);
        void module_check();
        int readInt();
        string readSymbol();
        string readIEAR();
        void print_sym_table();
        void print_mem_map();

        void createSymbol(string sym, int val);

        void do_op(string address_mode, int op);

        bool error_detected = false;
    private:
        int line_num = 0;
        int line_offset = 1;
        int line_offset_end = 1;
        int def_count_limit = 16;
        int use_count_limit = 16;
        int machine_memory_size = 512;
        int module_num = 1;
        int module_addr = 0;

        vector<string> used_sym;
        vector<string> global_used_sym;
        vector<string> use_list;
        vector<string> global_use_list;
        vector<vector<string> > all_use_list;
        vector<vector<string> > all_sym_list;
        vector<vector<string> > all_used_sym;

    vector<string> sym_list;
        vector<int> sym_address;

        ifstream f_in;
        stringstream line_stream;
        istringstream str_stream;
        vector<string> tokens;
        string token;
        vector<int> poss;
        string line_str;

        int token_index = 0;
        map<string,int> sym_table;


        vector<int> mem_map;

        vector<string> multi_defiend_syms;
        map<string,int>::iterator sym_table_iter;

        map<int, string> error_map;
        map<int, string> warnning_map;

        void check_addr(int addr);
        void check_relative_addr();
        int inst_count = 0;
        void init();

    public:
        Tokenizer(string file_name); // create tokenizer with file_name
        void tokenize();           // pass 1
        void tokenize_pass2(); // pass 2
};

bool Tokenizer::eof(ifstream& f_in)
{
    auto offset = f_in.tellg();
    string temp_line_str;
    if(token_index != tokens.size())
    {
        return false;
    }
    int temp_line_offset = 1;
    int temp_line_num = 0;

    while(temp_line_str.empty())
    {
        if(!getline(f_in,temp_line_str))
        {
            line_num += temp_line_num;
            line_offset = 1;
            return true;
        }
        temp_line_num +=1;
    }
    f_in.seekg(offset);
    return false;
}

void Tokenizer::check_relative_addr()
{
    sym_table_iter = sym_table.begin();
    while(sym_table_iter != sym_table.end()) 
    {
        string sym = sym_table_iter->first;
        int absolute_addr = sym_table_iter->second;
        int relative_addr = absolute_addr - module_addr;
        if(relative_addr >= inst_count)
        {
            cout << "Warning: Module " << module_num << ": " << sym << " too big " << relative_addr << " (max=" << inst_count-1 << ") assume zero relative" << endl;
            sym_table[sym] = module_addr;
        }
        
        sym_table_iter++;
    }
}


Tokenizer::Tokenizer(string file_name)
{
    f_in.open(file_name);
}

void Tokenizer::check_addr(int addr)
{
    if(addr > machine_memory_size)
    {
        throw invalid_argument("ADDR_EXPECTED"); 
    }
}

void Tokenizer::print_sym_table()
{
    cout << "Symbol Table" << endl;
    sym_table_iter = sym_table.begin();
    while(sym_table_iter != sym_table.end()) 
    {
        string sym = sym_table_iter->first;
        int absolute_addr = sym_table_iter->second;
        int relative_addr = absolute_addr - module_addr;
        cout << sym << "=" << absolute_addr; 
        vector<string>::iterator it = find(multi_defiend_syms.begin(), multi_defiend_syms.end(), sym);
        if(it != multi_defiend_syms.end())
        {
            cout << " Error: This variable is multiple times defined; first value used";
        }
        cout << endl;
        sym_table_iter++;
    }
    cout << endl;
}

void Tokenizer::print_mem_map()
{
    cout << "Memory Map" << endl;
    for (int i = 0; i < mem_map.size(); i++)
    {
        cout << setw(3) << setfill('0') << i<< ": " << setw(4) << setfill('0') << mem_map[i];
        if(error_map.find(i)!=error_map.end())
        {
            cout<<error_map[i];
        }
        cout << endl;
        if(warnning_map.find(i)!=warnning_map.end())
        {
            cout<<warnning_map[i];
        }
    }
}


void Tokenizer::createSymbol(string sym, int val)
{
    sym_list.push_back(sym);
    sym_address.push_back(val);
    sym_table_iter = sym_table.find(sym);
	if (sym_table_iter!= sym_table.end()) 
    {
        multi_defiend_syms.push_back(sym);
	}
    else
    {
        sym_table[sym] = module_addr + val;
    }
}


bool can_get_valid_line(ifstream& f_in)
{
    string line_str = "";
    auto offset = f_in.tellg();
    while(line_str.empty() && getline(f_in,line_str))
    {

    }
    if(!line_str.empty())
    {
        f_in.seekg(offset);
        return true;
    }
    return false;
}

bool Tokenizer::getToken()
{
    if(token_index < tokens.size())
    {
        token = tokens[token_index];
        line_offset = poss[token_index];
        token_index += 1;
        return true;
    }
    else
    {
        line_offset += token.length();
        token="";
        line_str="";
        while(line_str.empty())
        {
            if(!getline(f_in,line_str))
            {
                return false;
            }
            line_offset = 1;
            line_num +=1;
        }
        
        // reset token
        tokens.clear();
        poss.clear();
        token="";
        token_index = 0;
        get_tokens(" \t\n");
        token = tokens[token_index];
        line_offset = poss[token_index];
        token_index += 1;
        return true;
    }
}


string Tokenizer::readIEAR()
{
    if(getToken())
    {
        if(isIEAR(token))
        {
            return token;
        }
        else
        {
            throw invalid_argument("ADDR_EXPECTED");
        }
    }
    else
    {
        line_offset += token.length();
        throw invalid_argument("ADDR_EXPECTED");
        return "";
    }
}

int Tokenizer::readInt()
{
    if(getToken())
    {
        if(isNumber(token))
        {
            stringstream ss(token);
            int num = 0;
            ss >> num;
            return num;
        }
        else
        {
            throw invalid_argument("NUM_EXPECTED");
        }
    }
    else
    {
        // cout<<token.length();
        line_offset += token.length();
        throw invalid_argument("NUM_EXPECTED");
        return -1;
    }
}

string Tokenizer::readSymbol()
{
    if(getToken())
    {
        if(isSymbol(token))
        {
            return token;
        }
        else
        {
            throw invalid_argument("SYM_EXPECTED");
        }
    }
    else
    {
        line_offset += token.length();
        throw invalid_argument("SYM_EXPECTED");
    }
}


void Tokenizer::handle_parse_error(invalid_argument& e) 
{
    cout << "Parse Error line " << line_num << " offset " << line_offset << ": " << e.what() << endl;
    return;
}

void Tokenizer::tokenize()
{
    int mode = 0;
    while(!eof(f_in))
    {
        try
        {
            parse_tokens(mode);
            if(mode == 2)
            {
                check_relative_addr();
                module_addr += inst_count;
                module_num += 1;
            }
            mode += 1;
            mode = mode%3;
        }
        catch(invalid_argument& e) 
        {
            handle_parse_error(e);
            error_detected = true;
            return;
        }
    }
    // cout<<"Final Spot in File : line="<<line_num<<" offset="<<line_offset+token.length()<<endl;
    print_sym_table();
    return;
}

void Tokenizer::init()
{
    module_addr = 0;
    module_num = 1;
    sym_list.clear();
    use_list.clear();
    used_sym.clear();
    f_in.clear();
    f_in.seekg(0, f_in.beg);
}

void Tokenizer::module_check()
{

    for(int j = 0; j< all_use_list.size();j++) 
    {
        // rule 7
        use_list = all_use_list[j];
        used_sym = all_used_sym[j];
        for (int i = 0; i < use_list.size(); i++) {
            string sym = use_list[i];
            if (find(used_sym.begin(),used_sym.end(),sym) == used_sym.end()) // in used list but not used
            {
                string error_temp = "Warning: Module " + to_string(j+1) + ": " + use_list[i] +" appeared in the uselist but was not actually used\n";
                warnning_map[i] = error_temp;
            }
        }
        // if(module_error_info!="")
        // {
        //     module_error_info+="\n";
        // }

        // rule 4
        sym_list = all_sym_list[j];
        
        for (int i = 0; i < sym_list.size(); i++) 
        {
            string sym = sym_list[i];
            if(find(global_use_list.begin(),global_use_list.end(),sym)==global_use_list.end())
            {
                string error_temp = "Warning: Module " + to_string(j+1) + ": " + sym_list[i] +" was defined but never used\n";
                module_error_info += error_temp;
            }
        }
    }
}

void Tokenizer::tokenize_pass2()
{
    int mode = 0;
    init();
    while(!eof(f_in))
    {
        try
        {
            parse_tokens_pass2(mode);
            if(mode == 2)
            {
                // check_relative_addr();
                // post_process 
                
                // reset
                all_used_sym.push_back(used_sym);
                all_use_list.push_back(use_list);
                all_sym_list.push_back(sym_list);
                sym_list.clear();
                use_list.clear();
                used_sym.clear();
                module_addr += inst_count;
                module_num += 1;
            }
            //module reset
            mode += 1;
            mode = mode%3;
            
        }
        catch(invalid_argument& e)
        {
            handle_parse_error(e);
            error_detected = true;
            return;
        }
    }

//    cout<<"Final Spot in File : line="<<line_num<<" offset="<<line_offset+token.length()<<endl;
//    print_sym_table();
    module_check();
    print_mem_map();
    if(module_error_info != "")
    {
        cout<<module_error_info<<endl;
    }
//    print_module_error();
    return;
}

void Tokenizer::parse_tokens(int parse_mode = 0)
{
    if(parse_mode == 0)         //def_count
    {
        int def_count = readInt();
        if (def_count > def_count_limit)
        {
            throw invalid_argument("TOO_MANY_DEF_IN_MODULE");
        }
        for (int i = 0; i < def_count; i++)
        {
            string sym = readSymbol();
            int val = readInt();
            createSymbol(sym,val);
        }
    }
    else if (parse_mode == 1)  // use count
    {
        
        int use_count = readInt();
        if (use_count > use_count_limit)
        {
            throw invalid_argument("TOO_MANY_USE_IN_MODULE");
        }
        for (int i = 0; i < use_count; i++)
        {
            string sym = readSymbol();
            // use_list.push_back(sym);
        }
    }
    else if (parse_mode == 2)
    {
        inst_count = readInt();
        if (module_addr + inst_count > machine_memory_size)
        {
            throw invalid_argument("TOO_MANY_INSTR");
        }
        for (int i=0;i<inst_count;i++)
        {
            string address_mode = readIEAR();
            int op = readInt();
        }
    }
    return;
}


void Tokenizer::do_op(string address_mode, int op)
{
    // do_op
    int opcode = op / 1000;
    int operand = op % 1000;
    string err_info = "";
    if (address_mode == "I" && op >= 10000)    //! I model check
    {
        opcode = 9;
        operand = 999;
        err_info += " Error: Illegal immediate value; treated as 9999";
    }
    if (address_mode == "R")     //! relative mode
    {
        if (operand >= inst_count)
        {
            operand = 0;  // set to zero
            err_info += " Error: Relative address exceeds module size; zero used";
        }
        operand += module_addr;
    }
    if (address_mode == "E")
    {
        if (operand >= use_list.size())
        {
            // cout<<use_list.size()<<"size"<<endl;
            err_info += " Error: External address exceeds length of uselist; treated as immediate";
        }
        else if(sym_table.find(use_list[operand])== sym_table.end())
        {
            used_sym.push_back(use_list[operand]);
            global_used_sym.push_back(use_list[operand]);
            err_info = " Error: " + use_list[operand] + " is not defined; zero used";
            operand = 0;
        }
        else
        {
            // cout<<use_list;
            used_sym.push_back(use_list[operand]);
            global_used_sym.push_back(use_list[operand]);
            operand = sym_table[use_list[operand]];
        }
    }
    if (address_mode == "A")
    {
        if (operand >= machine_memory_size)
        {
            operand = 0;
            err_info = " Error: Absolute address exceeds machine size; zero used";
        }
    }
    // general check
    if (opcode >= 10) // err 11 general check
    {
        opcode = 9;
        operand = 999;
        err_info += " Error: Illegal opcode; treated as 9999";
    }
    op = opcode * 1000 + operand;
    mem_map.push_back(op);
    if(err_info!="")
    {
        error_map[mem_map.size() - 1] = err_info;
    }
}


void Tokenizer::parse_tokens_pass2(int parse_mode = 0)
{
    if(parse_mode == 0)         //def_count
    {
        int def_count = readInt();
        if (def_count > def_count_limit)
        {
            throw invalid_argument("TOO_MANY_DEF_IN_MODULE");
        }
        for (int i = 0; i < def_count; i++)
        {
            string sym = readSymbol();
            int val = readInt();
            createSymbol(sym,val);
        }
    }
    else if (parse_mode == 1)  // use count
    {

        int use_count = readInt();
        if (use_count > use_count_limit)
        {
            throw invalid_argument("TOO_MANY_USE_IN_MODULE");
        }
        for (int i = 0; i < use_count; i++)
        {
            string sym = readSymbol();
            use_list.push_back(sym);
            global_use_list.push_back(sym);
        }
    }
    else if (parse_mode == 2)
    {
        inst_count = readInt();
        if (module_addr + inst_count > machine_memory_size)
        {
            throw invalid_argument("TOO_MANY_INSTR");
        }
        for (int i=0;i<inst_count;i++)
        {
            string address_mode = readIEAR();
            int op = readInt();
            do_op(address_mode, op);
        }
    }

    return;
}


void Tokenizer::get_tokens(const std::string delim = " \t\n")
{
    //reset token and token index
    // tokens.clear();                                     //global private

    // token_index = 0;
    auto start = line_str.find_first_not_of(delim, 0);
    auto position = line_str.find_first_of(delim, start);
    string token;
    while (position != std::string::npos || start != std::string::npos)
    {
        // [start, position)
        token = line_str.substr(start, position - start);
        // cout<<"Token: "<<line_num<<":"<<start+1<<" : "<<token<<endl;
        poss.push_back(start+1);
        tokens.emplace_back(std::move(token));
        start = line_str.find_first_not_of(delim, position);
        position = line_str.find_first_of(delim, start);
    }
}
