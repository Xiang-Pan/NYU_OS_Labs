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
    return 0;
}
