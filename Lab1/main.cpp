#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <istream>
#include <vector> 
#include <regex>
#include "tokenizer.hpp"
using namespace std;



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
// template <typename Enumeration>
// auto as_integer(Enumeration const value)-> typename std::underlying_type<Enumeration>::type
// {
//      return static_cast<typename std::underlying_type<Enumeration>::type>(value);
// }





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
    if(tokenizer.error_detected)
    {
        return -1;
    }
    tokenizer.tokenize_pass2();
    // tokenizer.readSymbol();
    // string a = "aaa";
    // isSymbol(a);
    return 0;
}
