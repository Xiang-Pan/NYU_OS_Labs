/*
 * @Author: Xiang Pan
 * @Date: 2021-07-14 02:04:44
 * @LastEditTime: 2021-07-20 02:56:30
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab4/input_handler.hpp
 * xiangpan@nyu.edu
 */
#include <iostream>
#include <string>
#include <ostream>
#include <fstream>
#include <vector>
#include <unistd.h>

#include "io_scheduler.hpp"

using namespace std;

class InputHandler
{
    public:
        // ProcessManager process_manager;
        // #numio=10 maxtracks=128 lambda=0.100000
        int arg_parse(int argc, char* argv[]);
        InputHandler(int argc, char* argv[])
        {
            int num = arg_parse(argc, argv);
            debug(num);
            debug(argc);
            if((num+2) != argc)
            {
                cout << "please input file name!" << endl;
            }
            inputfile_stream.open((argv[argc-1])); //
            read_input_file();
            
        }
        
        vector<string> get_tokens(string line_str,const std::string delim);


        ifstream inputfile_stream;
        bool read_input_line();
        void read_input_file();
        bool read_instruction();

        vector<string> tokens_;
        int line_num_ = 0;


        IOScheduler* p_io_scheduler;

        void simulate();
        void summary();
    private:
};

vector<string> InputHandler::get_tokens(string line_str,const std::string delim = " \t\n")
{
    auto start = line_str.find_first_not_of(delim, 0);
    auto position = line_str.find_first_of(delim, start);
    string token;
    vector<string> tokens;
    while (position != std::string::npos || start != std::string::npos)
    {
        // [start, position)
        token = line_str.substr(start, position - start);
        // poss.push_back(start+1);
        tokens.emplace_back(std::move(token));
        start = line_str.find_first_not_of(delim, position);
        position = line_str.find_first_of(delim, start);
    }
    return tokens;
}

bool InputHandler::read_input_line()
{
    string line;
    bool succ = static_cast<bool>(getline(inputfile_stream, line));
    if(succ)
    {
        tokens_ = get_tokens(line);
        line_num_ +=1; 
    }
    debug(line);
    return succ;
}


void InputHandler::read_input_file()
{
    if(read_input_line() && tokens_[tokens_.size()-1] == "generator")
    {
        while(read_input_line() && tokens_[0] == "#")
        {
            // get parameters from
            debug("here");
            debug_vector(tokens_);
            //continue the commend line
        }
        // debug_vector(tokens_);
        // process_count_ = string2int(tokens_[0]);
    }
}


bool InputHandler::read_instruction()
{
    if(read_input_line() && tokens_[0][0] != '#')
    {
        // debug(tokens_[0]);
        // op = tokens_[0][0];
        // vpage = string2int(tokens_[1]);
        // // cout<<line_num_<<":"<<op<<vpage<<endl;
        // inst_count_ += 1;
        // kInstCount = inst_count_;
        // return true;
    }
    else
    {
        debug("read_end");
        return false;
    }
    return false;
}

int InputHandler::arg_parse(int argc, char* argv[])
{
    int num = 0;
    int ch;
    const char *optstring = "s:"; 
    string opt;
    while ((ch = getopt(argc, argv, optstring)) != -1) 
    {
        num += 1;
        switch(ch)
        {
            case 's': 
            {
                switch (optarg[0])
                {
                    case 'i': 
                    {
                        debug("FIFOIOScheduler");
                        p_io_scheduler = new FIFOIOScheduler();
                        break;
                    }
                    // case 'j': 
                    // {
                    //     p_io_scheduler = new SSTFIOScheduler();
                    //     break;
                    // }
                    // case 's': 
                    // {
                    //     p_io_scheduler = new LOOKIOScheduler();
                    //     break;             
                    // }
                    // case 'c': 
                    // {
                    //     p_io_scheduler = new CLOOKIOScheduler();
                    //     break;
                    // }
                    // case 'f': 
                    // {
                    //     p_io_scheduler = new FLOOKIOScheduler();
                    //     break;
                    // }
                    default:
                        break;
                }
                break;
            }
            case 'v': 
            {
                break;
            }

        }
    }
    return num;
}




void InputHandler::simulate()
{
    while(read_instruction())
    {
        
    }
}

void InputHandler::summary()
{
    
}