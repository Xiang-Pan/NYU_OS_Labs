/*
 * @Author: Xiang Pan
 * @Date: 2021-06-23 17:42:15
 * @LastEditTime: 2021-07-13 02:21:52
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/input_handler.hpp
 * xiangpan@nyu.edu
 */
#pragma once
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
#include "process_manager.hpp"
#include "process.hpp"
#include "pager.hpp"
#include "output_handler.hpp"
using namespace std;


bool output_O = false;
bool output_pagetable = false;
bool output_frametable = false;
bool output_summary = false;


bool output_current_pagetable_after_instruction = false; // x 
bool output_all_pagetable_after_instruction = false;     // y
bool output_framtable_after_instruction = false;         // f
bool output_aging_info = false;                         // aging


class InputHandler
{
    public:
        ProcessManager process_manager;
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
        int process_count_ = -1;

        ifstream inputfile_stream;

        int frame_num = -1;
        frame_t** frame_table; // frame_table



        bool read_instruction();
        int inst_count = 0;
        string op;
        int vpage;
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

    // get process count
    if(read_input_line() && tokens_[tokens_.size()-1] == "generator")
    {
        while(read_input_line() && tokens_[0] == "#")
        {
            //continue the commend line
        }
        debug_vector(tokens_);
        process_count_ = string2int(tokens_[0]);
    }

    // set process info
    // i. the number of virtual memory areas / segments (aka VMAs)
    // ii. specification for each said VMA comprised of 4 numbers:
    // “starting_virtual_page ending_virtual_page write_protected[0/1] filemapped[0/1]”
    // while(read_input_line() && tokens_[1] == "process")
    for(int i = 0; i < process_count_; i++)
    {
        // Process *process = new Process()
        read_input_line();
        int cur_process_num = string2int(tokens_[2]);    
        Process p;
        read_input_line();
        read_input_line();
        p.pid = cur_process_num;
        p.VMA_count_ = string2int(tokens_[0]);
        for(int i = 0; i <p.VMA_count_; i++)
        {
            read_input_line();

            VMA v = VMA(string2int(tokens_[0]), string2int(tokens_[1]), string2bool(tokens_[2]), string2bool(tokens_[3]));
            p.add_VMA(v);
        }
        process_manager.add_process(p);
    }

}

bool InputHandler::read_instruction()
{
    if(read_input_line() && tokens_[0] != "#")
    {
        op = tokens_[0];
        vpage = string2int(tokens_[1]);
        // cout<<line_num_<<":"<<op<<vpage<<endl;
        inst_count += 1;
        return true;
    }
    else
    {
        debug("read_fail");
        return false;
    }
}


//! simulation
void InputHandler::simulate()
{
    
    debug("simulation");
    Process* cur_process;
    if(read_input_line() && tokens_[tokens_.size() - 2] == "simulation")
    {
        // debug_vector(tokens_);
        //! begin simulation
        while(read_instruction())
        { 
            
            // debug(output_O);
            if(output_O)
            {
                log_operation(inst_count, op, vpage);
            }
            
            // get pte
            pte_t* pte = &(cur_process->page_table_[vpage]);    // in reality this is done by hardware
            if(!pte->present) 
            {
                
                
                // this in reality generates the page fault exception and now you execute 
                // verify this is actually a valid page in a vma if not raise error and next inst frame_t *newframe = get_frame();

                //-> figure out if/what to do with old frame if it was mapped // see general outline in MM-slides under Lab3 header and writeup below // see whether and how to bring in the content of the access page.
                // check write protection 
                
                // simulate instruction execution by hardware by updating the R/M PTE bits update_pte(read/modify) bits based on operations.

            } 

            // update_pte(read/modify) bits based on operations.

            update_pte();


            

        }
    }

}


int InputHandler::arg_parse(int argc, char* argv[])
{
    int num = 0;
    int ch;
    const char *optstring = "f:a:o:"; 
    // string a;
    // string param;
    string opt;
    while ((ch = getopt(argc, argv, optstring)) != -1) 
    {
        printf("optind: %d\n", optind);
        num += 1;
        debug(ch);
        switch(ch)
        {
            case 'f':
                frame_num = string2int(optarg);
                debug(frame_num);
                break;
            case 'o':
                opt = string(optarg);
                // debug(opt);
                for (char& c : opt) 
                {
                    switch (c) 
                    {
                        
                        case 'O':
                            output_O = true;
                            break;
                        case 'P':
                            output_pagetable = true;
                            break;
                        case 'F':
                            output_frametable = true;
                            break;
                        case 'S':
                            output_summary = true;
                            break;
                        case 'a':
                            output_aging_info = true;
                            break;
                        case 'f':
                            output_framtable_after_instruction = true;
                            break;
                        case 'x':
                            output_current_pagetable_after_instruction = true;
                            break;
                        case 'y':
                            output_all_pagetable_after_instruction = true;
                            break;
                        default:
                            cerr << "unknown output option: `" << c << "'." << endl;
                            break;
                    }
                }
                break;
            case '?':
                printf("error optopt: %c\n", optopt);
                break;
        }
        
    }
    return num;
}
