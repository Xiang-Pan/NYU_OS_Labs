#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <istream>
#include <vector> 
#include <regex>
#include <unistd.h> //parse args
#include "utils.hpp"
#include "process.hpp"
#include "pager.hpp"
using namespace std;


class InputHandler
{
    public:
        InputHandler(int argc, char* argv[])
        {
            
            arg_parse(argc, argv);
            inputfile_stream.open((argv[argc-1])); //
            read_input_file();
            
        }
        vector<string> tokens_;
        int arg_parse(int argc, char* argv[]);
        bool read_input_line();
        void read_input_file();
        vector<string> get_tokens(string line_str,const std::string delim);

        int line_num_ = 0;

        ifstream inputfile_stream;

        int frame_num = -1;
        frame_t** frame_table; // frame_table
        // pte_t page_table[MAX_VPAGES];


        void simulate();
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
    return succ;
}

void InputHandler::read_input_file()
{
    string line;
    int pid = 0;

    // get process count
    if(read_input_line() && tokens_[tokens_.size()-1] == "generator")
    {
        while(read_input_line() && tokens_[0] == "#")
        {
            //continue the commend line
        }
        debug_vector(tokens_);
        int process_count = string2int(tokens_[0]);
    }

    // set process info
    // i. the number of virtual memory areas / segments (aka VMAs)
    // ii. specification for each said VMA comprised of 4 numbers:
    // “starting_virtual_page ending_virtual_page write_protected[0/1] filemapped[0/1]”
    while(read_input_line() && tokens_[1] == "process")
    {
        // Process *process = new Process()
        int cur_process_num = string2int(tokens_[2]);    
        Process p;
        read_input_line();
        read_input_line();
        p.process_num_ = cur_process_num;
        p.VMA_count_ = string2int(tokens_[0]);
        for(int i = 0; i <p.VMA_count_; i++)
        {
            read_input_line();
            VMA v = VMA(string2int(tokens_[0]), string2int(tokens_[1]), string2bool(tokens_[2]), string2bool(tokens_[3]));
            p.add_VMA(v);
            // line_num += 1;
        }
    }

}


void InputHandler::simulate()
{
    string op;
    int vpage;
    if(read_input_line() && tokens_[tokens_.size() - 2] == "simulation")
    {
        // begin simulation
        while (read_input_line() && tokens_[0] != "#")
        {
            op = tokens_[0];
            vpage = string2int(tokens_[1]);
            // pte_t *pte = &current_process.page_table[vpage];// in reality this is done by hardware

        }

        
        
    }

}


int InputHandler::arg_parse(int argc, char* argv[])
{
    int num = 0;
    int o = 0;
    const char *optstring = "f:a:o:"; 
    string a;
    string param;
    while ((o = getopt(argc, argv, optstring)) != -1) 
    {
        num += 1;
        switch (o)
        {
            case 'f':
                frame_num = string2int(optarg);
                debug(frame_num);
                break;
            case '?':
                printf("error optopt: %c\n", optopt);
                printf("error opterr: %d\n", opterr);
                break;
        }
    }
    return num;
}
