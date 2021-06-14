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
using namespace std;

// enum PARSE_ERROR_TYPE 
// {
//     NUM_EXPECTED,
//     SYM_EXPECTED,
//     ADDR_EXPECTED,
//     SYM_TOO_LONG,
//     TOO_MANY_DEF_IN_MODULE,
//     TOO_MANY_USE_IN_MODULE,
//     TOO_MANY_INSTR
// };


bool isNumber(const string& str)
{
    cout<<str<<endl;
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
    return token=="I" || token=="A" || token=="E" || token=="R";
}



class Tokenizer 
{
    public:
        void get_tokens(const std::string delim);
        bool getToken();
        void parse_tokens(int parse_mode);
        void handle_parse_error(invalid_argument& e);

        int readInt();
        string readSymbol();
        string readIAER();
        string readIEAR();


        void createSymbol(string sym, int val);
    private:
        int line_num = 1;
        int line_offset = 1;
        int line_offset_end = 1;
        int def_count_limit = 16;
        int use_count_limit = 16;
        int machine_memory_size = 512;
        int module_num = 1;
        int module_addr = 0;

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
        map<string,int>::iterator sym_table_iter;


        


    public:
        Tokenizer(string file_name); // create tokenizer for file_name
        // getLine();
        // getToken();
        void tokenize();
};


Tokenizer::Tokenizer(string file_name)
{
    f_in.open(file_name);
}


void Tokenizer:: createSymbol(string sym, int val)
{
    sym_list.push_back(sym);
    sym_address.push_back(val);
    // check sym in sym_table
    // map<sting,string>::iterator iter;
    sym_table_iter = sym_table.find(sym);
	if (sym_table_iter!= sym_table.end()) 
    {
        // sym in table alread
		cout << "字典中有这个值" << endl;
	}
    else
    {
        sym_table[sym] = module_addr + val;
    }
}

bool Tokenizer::getToken()
{
    if(token_index < tokens.size())
    {
        token = tokens[token_index];
        token_index += 1;
        line_offset = poss[token_index];
        return true;
    }
    else
    {
        return false;
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
        throw invalid_argument("ADDR_EXPECTED");
        return "";
    }
}

//bool Tokenizer::

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
            return "";
        }
    }
    else
    {
        return "";
    }
}


void Tokenizer::handle_parse_error(invalid_argument& e) 
{
    cout << "Parse Error line " << line_num << " offset " << line_offset << ": " << e.what() << endl;
    return;
}



void Tokenizer::tokenize()
{
    bool eof = false;
    for(; getline(f_in,line_str);)
    {   
        //create module 
        int mode = (line_num-1)%3;
        get_tokens(" ");
        try
        {
            parse_tokens(mode);
        }
        catch(invalid_argument& e) 
        {
            handle_parse_error(e);
            return;
        }
        //create sym table
        if(mode == 2) // 
        {
            for(int i = 0; i < sym_list, i++)
            {
                string sym = sym_list[i] 
                int addr = sym_address[i] 
            }
        } 


        line_num++;
        cout<< line_num <<endl;
    }
    return;
}


// void Tokenizer::

void Tokenizer::parse_tokens(int parse_mode = 0)
{
    //def_count
    if(parse_mode == 0)
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
    else if (parse_mode == 1)
    {
        // use count
        int use_count = readInt();
        if (use_count > use_count_limit)
        {
            throw invalid_argument("TOO_MANY_USE_IN_MODULE");
        }
        for (int i = 0; i < use_count; i++)
        {
            readSymbol();
        }
    }
    else if (parse_mode == 2)
    {
        int inst_count = readInt();
        if (module_addr + inst_count > machine_memory_size)
        {
            throw invalid_argument("TOO_MANY_INSTR");
        }
        for (int i=0;i<inst_count;i++)
        {
            string address_mode = readIEAR();
            int operand = readInt();
        }
        module_addr += inst_count;
        module_num += 1;
    }





    // this would change in pass2
    

    return;
}

void Tokenizer::get_tokens(const std::string delim = " ")
{
    //reset token and token index
    tokens.clear();                                     //global private
    token_index = 0;
    auto start = line_str.find_first_not_of(delim, 0);
    auto position = line_str.find_first_of(delim, start);
    string token;
    while (position != std::string::npos || start != std::string::npos)
    {
        // [start, position)
        token = line_str.substr(start, position - start);
        cout<<"Token: "<<line_num<<":"<<start+1<<" : "<<token<<endl;
        poss.push_back(start+1);
        tokens.emplace_back(std::move(token));
        start = line_str.find_first_not_of(delim, position);
        position = line_str.find_first_of(delim, start);
    }

}

