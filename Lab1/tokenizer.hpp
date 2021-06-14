#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <istream>
#include <vector> 
#include <regex>
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
    return isSym && checkSymLen;
}

bool isIAER(const string& token) 
{
    return token=="I" || token=="A" || token=="E" || token=="R";
}



class Tokenizer 
{
    public:
        void get_tokens(const std::string delim);
        bool getToken();
        void parse_tokens();
        void handle_parse_error(invalid_argument& e);

        int readInt();
        string readSymbol();
        void createSymbol(string sym, int val);
        string readIAER();
    private:
        int line_num = 1;
        int line_offset = 1;
        int line_offset_end = 1;

        ifstream f_in;
        stringstream line_stream;
        istringstream str_stream;
        vector<string> tokens;
        string token;
        vector<int> poss;
        string line_str;

        int token_index;


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
    vector<string> tokens;
    for(; getline(f_in,line_str);)
    {
        get_tokens(" ");
        try
        {
            parse_tokens();
        }
        catch(invalid_argument& e) 
        {
            handle_parse_error(e);
            return;
        }
        line_num++;
        cout<< line_num <<endl;
    }
    return;
}


// void Tokenizer::

void Tokenizer::parse_tokens()
{
//    if(isNumber(tokens[0]) != true)
//    {
//        throw invalid_argument("NUM_EXPECTED");
//    }
    // defNum convert to int
    int def_count = readInt();
    for (int i = 0; i < def_count; i++)
    {
         string sym = readSymbol();
         int val = readInt();
         createSymbol(sym,val);
        // string symbol = tokenizer.read_symbol();
        // int rel_addr = tokenizer.read_int();
        // deflist.push_back(symbol);
        // defaddr.push_back(rel_addr);
    }
    // stringstream geek(s);

    return;
    
}


//void split(const std::string& str, std::vector<std::string>& tokens, const std::string delim = " ")
//{
//    tokens.clear();                                     //global private
//    auto start = str.find_first_not_of(delim, 0);       // 分割到的字符串的第一个字符
//    auto position = str.find_first_of(delim, start);    // 分隔符的位置
//    string token;
//    while (position != std::string::npos || start != std::string::npos) {
//        // [start, position) 为分割下来的字符串
//        token = str.substr(start, position - start);
//        tokens.emplace_back(std::move(token));
//        start = str.find_first_not_of(delim, position);
//
//        position = str.find_first_of(delim, start);
//    }
//}



void Tokenizer::get_tokens(const std::string delim = " ")
{
    tokens.clear();                                     //global private
    auto start = line_str.find_first_not_of(delim, 0);       // 分割到的字符串的第一个字符
    auto position = line_str.find_first_of(delim, start);    // 分隔符的位置
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

