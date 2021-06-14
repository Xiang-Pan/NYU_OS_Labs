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
    std::string pattern{"[a-zA-Z][0-9a-zA-Z]*"}; 
	std::regex re(pattern);
    bool ret = std::regex_match(token, re);
    // cout<<ret<<endl;
    return ret;
}

bool isIAER(const string& token) 
{
    return token=="I" || token=="A" || token=="E" || token=="R";
}



class Tokenizer 
{
    public:
        vector<string> get_tokens(const string &str, const string &pattern);
        void parse_tokens(vector<string> tokens);
        void handle_parse_error(invalid_argument& e);

        int readInt();
        string readSymbol();
        string readIAER();
    private:
        int line_num = 1;
        int line_offset = 1;
        int line_offset_end = 1;

        ifstream f_in;
        stringstream line_stream;
        istringstream str_stream;
        vector<string> tokens;
        vector<int> poss;
        string line;
        string token;
    public:
        Tokenizer(string file_name); // create tokenizer for file_name
        getLine();
        getToken();
        void tokenize();
};

int Tokenizer::readInt()
{

}


bool Tokenizer::getToken() 
{
    while (!(iss_ >> token_)) {
        if (!loadline()) {
            lineoffset_ = finalpos_;
            return false;
        }
    }
    lineoffset_ = static_cast<int>(iss_.tellg()) - static_cast<int>(token_.size()) + 1;
    return true;
}



void Tokenizer::handle_parse_error(invalid_argument& e) 
{
    int line_num = 1;
    int line_offset = 1; 
    cout << "Parse Error line " << line_num << " offset " << line_offset << ": " << e.what() << endl;
    return;
}

Tokenizer::Tokenizer(string file_name)
{
    f_in.open(file_name);
}

void Tokenizer::tokenize()
{
    vector<string> tokens;
    for(string line; getline(f_in,line);)
    {
        tokens = get_tokens(line);
        try
        {
            parse_tokens(tokens);
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

void Tokenizer::parse_tokens(vector<string> tokens)
{
    if(isNumber(tokens[0]) != true)
    {
        throw invalid_argument("NUM_EXPECTED");
    }
    // defNum convert to int
    stringstream ss(tokens[0]);
    int defNum = 0;
    ss >> defNum;
    cout<<defNum<< endl;
    for (int i = 0; i < defNum; i++) 
    {
        string sym = readSym();
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



vector<string> Tokenizer::get_tokens(const string &str, const string &pattern)
{
    vector<string> tokens;
    if(str == "")
        return tokens;
    string strs = str + pattern;
    size_t pos = strs.find(pattern);
    int offset = 1;
    while(pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        tokens.push_back(temp);
        strs = strs.substr(pos+1, strs.size());
        pos = strs.find(pattern);
    }
    return tokens;
    // int offset = 1;
    // string temp_str;
    // stringstream line_stream(line);
    // vector <string> tokens;
    // vector <int> offset;
    // while(getline(line_stream, temp_str, ' '))
    // {
    //     tokens.push_back(temp_str);

    //     offset.push_back(offset);

    // }
    // return tokens;
}

