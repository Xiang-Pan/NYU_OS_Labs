/*
 * @Author: Xiang Pan
 * @Date: 2021-06-23 17:42:15
 * @LastEditTime: 2021-07-20 00:59:26
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
#include "system_shared.hpp"
#include "process.hpp"
#include "pager.hpp"
#include "output_handler.hpp"
using namespace std;


// extern ProcessManager* p_process_manager_;

class InputHandler
{
    public:
        // ProcessManager process_manager;
        ProcessManager* p_process_manager_ = new ProcessManager();
        Pager* p_pager_;
        
        InputHandler(int argc, char* argv[])
        {
            
            int arg_num = arg_parse(argc, argv);
            // debug(arg_num);
            // debug(argc);
            if((arg_num + 2) != argc)
            {
                debug(arg_num);
                debug(argc);
                debug("please input file name");
                // cout<<<<endl;
            }
            inputfile_stream.open((argv[argc-2])); //
            randomfile_stream.open((argv[argc-1])); //
            read_randomfile();
            read_input_file();
            
            // Pager(); 

            
        }
        vector<string> tokens_;
        int arg_parse(int argc, char* argv[]);
        bool read_input_line();
        void read_input_file();
        vector<string> get_tokens(string line_str,const std::string delim);

        int line_num_ = 0;
        int process_count_ = -1;
        int context_switches_ = 0;
        int process_exits_ = 0;
        int cycles_ = 0;

        ifstream inputfile_stream;
        // ifstream randomfile_stream;

        // int frame_num = -1;
        frame_t** frame_table; // frame_table



        bool read_instruction();
        int inst_count_ = -1;
        char op;
        int vpage;
        // pte_t page_table[MAX_VPAGES];


        void simulate();
        void summary();
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
        // debug_vector(tokens_);
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
        p.pid_ = cur_process_num;
        p.VMA_count_ = string2int(tokens_[0]);
        for(int i = 0; i <p.VMA_count_; i++)
        {
            read_input_line();
            VMA v = VMA(string2int(tokens_[0]), string2int(tokens_[1]), string2bool(tokens_[2]), string2bool(tokens_[3]));
            p.add_VMA(v);
        }
        p_process_manager_->add_process(p);
    }

}

bool InputHandler::read_instruction()
{
    if(read_input_line() && tokens_[0][0] != '#')
    {
        // debug(tokens_[0]);
        op = tokens_[0][0];
        vpage = string2int(tokens_[1]);
        // cout<<line_num_<<":"<<op<<vpage<<endl;
        inst_count_ += 1;
        kInstCount = inst_count_;
        return true;
    }
    else
    {
        debug("read_end");
        return false;
    }
}


//! simulation
void InputHandler::simulate()
{
    
    debug("simulation");
    
    if(read_input_line() && tokens_[tokens_.size() - 2] == "simulation")
    {
        // debug_vector(tokens_);
        //! begin simulation
        bool page_fault_handler_result = true;
        while(read_instruction())
        {
            Process& cur_process = p_process_manager_->get_cur_process(); 
            log_operation(inst_count_, op, vpage);
            page_fault_handler_result = true;
            
            // get pte
            pte_t& pte = cur_process.get_pte(vpage);    // in reality this is done by hardware

            // do op
            switch(op)
            {
                case 'c':
                {
                    p_process_manager_->cur_pid = vpage; // pid actually 
                    total_cost += kContextSwitches;
                    context_switches_ += 1;
                    break;
                }
                case 'r':
                {
                    total_cost += kReadWrite;
                    pte.referenced = true;

                    //check present first
                    if(pte.present == false) 
                    {
                        page_fault_handler_result = p_pager_->page_fault_handler(pte, vpage, cur_process);
                        if(page_fault_handler_result == false)
                        {
                            break;
                        }
                        // break;
                    }
                    pte.referenced = true;

                    if(pte.in_VMA == false)
                    {
                        cur_process.segprot_ += 1;
                        total_cost += kSegprot;
                        log_segprot();
                    }
                    break;


                    
                }
                case 'w':
                {
                    total_cost += kReadWrite;

                    //check present fir
                    pte.referenced = true;
                    if(pte.present == false)  // is valid
                    {
                        pte.modified = false;
                        page_fault_handler_result = p_pager_->page_fault_handler(pte, vpage,cur_process); // this may reset pte
                        if(page_fault_handler_result == false)
                        {
                            break;
                        }
                    }
                    pte.referenced = true;
                    if(pte.write_protected)
                    {
                        log_segprot();
                        cur_process.segprot_ += 1;
                        total_cost += kSegprot;
                    }
                    else
                    {
                        
                        pte.modified = true;
                    }
                    break;
                }
                case 'e':
                {
                    process_exits_ += 1;
                    cout << "EXIT current process " << vpage << endl;
                    total_cost += kProcessExits;
                    cur_process.umap_all();
                    break;
                }
            }
            if(output_current_pagetable_after_instruction) // x
            {
                if(op == 'c' or op == 'e')
                {
                    continue;
                }
                if(page_fault_handler_result)
                {
                    cur_process.print_page_table();
                }
            }
            if(output_framtable_after_instruction)
            {
                print_frame_table();
            }
        }
    }

}


int InputHandler::arg_parse(int argc, char* argv[])
{
    int num = 0;
    int ch;
    const char *optstring = "f:a:o:"; 
    string opt;
    while ((ch = getopt(argc, argv, optstring)) != -1) 
    {
        // printf("optind: %d\n", optind);
        num += 1;
        switch(ch)
        {

            case 'f':
                frame_table_size = string2int(optarg);
                // debug(frame_num);
                break;
            case 'a':
            {
                switch(*optarg)
                {
                    case 'a':
                        debug("AgingPager");
                        p_pager_ = new AgingPager(p_process_manager_);
                        break;
                    case 'f':
                        p_pager_ = new FIFOPager(p_process_manager_);
                        break;
                    case 'r':
                        p_pager_ = new RandomPager(p_process_manager_);
                        break;
                    case 'e':
                        p_pager_ = new NRUPager(p_process_manager_);
                        break;
                    case 'c':
                        p_pager_ = new ClockPager(p_process_manager_);
                        break;
                    case 'w':
                        p_pager_ = new WorkingSetPager(p_process_manager_);
                        break;
                }
                break;
            }
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
                            output_aging_info = true; // a 
                            break;
                        case 'f':
                            output_framtable_after_instruction = true; // after each instruction
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


void InputHandler::summary()
{
    if(output_pagetable)
	{
        p_process_manager_->print_all_page_able();
	}
    if(output_frametable)
    {
        print_frame_table();
    }
	if(output_summary)
	{
        // cout<<"summary"<<endl;
        p_process_manager_->print_all_process_stats();

        cout << "TOTALCOST " << (inst_count_+1) << " "
                             << context_switches_ << " "
                             << process_exits_ << " "
                             << total_cost << " "
                             << sizeof(pte_t) << endl;
    }
        // printf("TOTALCOST %lu %lu %lu %llu\n", (inst_count_ + 1), context_switches_, total_cost, process_exits_);
        
        // PROC[0]: U=10 M=26 I=0 O=4 FI=0 FO=0 Z=26 SV=0 SP=0
        // TOTALCOST 31 1 0 26400 4
		// Output_stats(processQueue);
		// Output_Summary();
}
