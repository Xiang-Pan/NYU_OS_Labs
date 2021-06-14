#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <istream>
#include <vector> 
#include <regex>
// #include <linker.h>
// #include "linker.h"
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
    std::string pattern{ "[a-Z][a-Z0-9]*" }; 
	std::regex re(pattern);
    bool ret = std::regex_match(token, re);
    return ret;
}

bool isIAER(const string& token) 
{
    return token=="I" || token=="A" || token=="E" || token=="R";
}



class Linker 
{
    
    private:
        void print_symbol_table();
        void print_memory_map();
};



// void Linker::print_symbol_table(vector<string>& def_list, unordered_map<string, bool> md) 
// void Linker::print_symbol_table() 

// {
    // cout << "Symbol Table" << endl;
    // for (string& symbol : deflist) 
    // {
    //     cout << symbol << "=" << symbol_table_[symbol];
    //     if (md[symbol]) {
    //         cout << " Error: This variable is multiple times defined; first value used";
    //     }
    //     cout << endl;
    // }
    // cout << endl;


class Tokenizer 
{
    private:
        enum PARSE_ERROR_TYPE 
        {
            NUM_EXPECTED,
            SYM_EXPECTED,
            ADDR_EXPECTED,
            SYM_TOO_LONG,
            TOO_MANY_DEF_IN_MODULE,
            TOO_MANY_USE_IN_MODULE,
            TOO_MANY_INSTR
        };
        
        int cur_line_num = 0;
        int cur_line_offset = 0;
        ifstream f_in;
        istringstream str_stream;
        string line;


        vector<string> get_tokens(string line);
        void parse_tokens(vector<string> tokens);
        void handle_parse_error(PARSE_ERROR_TYPE error_type);
        

    public:
        Tokenizer(string file_name); // create tokenizer for file_name
        void tokenize();
};


void Tokenizer::handle_parse_error(PARSE_ERROR_TYPE error_type) 
{
    int line_num = 0;
    int line_offset = 0; 
    cout << "Parse Error line " << line_num << " offset " << line_offset << ": " << error_type << endl;
    return;
}

Tokenizer::Tokenizer(string file_name)
{
    f_in.open(file_name);
    return;
}

void Tokenizer::tokenize()
{
    vector<string> tokens;
    for(string line; getline(f_in,line);)
    {
        // cout<<line<<endl;
        tokens = get_tokens(line);
        try
        {
            parse_tokens(tokens);
        }
        catch(PARSE_ERROR_TYPE error_type) 
        {
            handle_parse_error(error_type);
            return;
        }
        cur_line_num ++;
        cout<<cur_line_num<<endl;
    }
    return;
}

void Tokenizer::parse_tokens(vector<string> tokens)
{
    if(isNumber(tokens[0]) != true)
    {
        throw PARSE_ERROR_TYPE::NUM_EXPECTED;
    }

    // expect 
    return;
    
    // f_in.open(file_name);
    // vector<string> tokens;
    // for(string line; getline(f_in,line);)
    // {
    //     cout<<line<<endl;
    //     tokens = get_tokens(line);
    //     parse_tokens(tokens)
    // }
    // return;
}



vector<string> Tokenizer::get_tokens(string line)
{
    string temp_str;
    stringstream line_stream(line);
    vector <string> tokens;
    while(getline(line_stream, temp_str, ' '))
    {
        tokens.push_back(temp_str);
    }
    return tokens;
}




int main(int argc, const char * argv[]) 
{
    // cout<<argc<<endl;
    if(argc!=2)
    {
        cout<<"Expected argument after options"<<endl;
        return -1;
    }

    Tokenizer tokenizer(argv[1]);
    tokenizer.tokenize();
    // tokenizer.parse

            // bool loadline() 
            // {
            //     string line;
            //     if (getline(ifs_, line)) 
            //     {
            //         finalpos_ = static_cast<int>(line.size()) + 1;
            //         iss_.clear();
            //         iss_.str(line + " ");
            //         linenum_++;
            //         return true;
            //     }
            //     return false;
            // }


    
    // Tokenizer tokenizer();
    // Linker linker();

    return 0;
}
