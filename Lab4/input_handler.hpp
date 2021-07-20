/*
 * @Author: Xiang Pan
 * @Date: 2021-07-14 02:04:44
 * @LastEditTime: 2021-07-20 03:56:05
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
#include <algorithm>
// #include <boost/algorithm/string.hpp>
#include <unistd.h>
#include "io_request.hpp"
#include "io_scheduler.hpp"
#include "system_shared.hpp"
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


        int numio_;
        int maxtracks_;
        float lambda_;


        int time_;
        int track_;


        ifstream inputfile_stream;
        bool read_input_line();
        void read_input_file();
        bool read_instruction();
        void read_all_instruction();

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
    if(line.empty())
    {
       return false;
    }
    debug(line);
    return succ;
}


void InputHandler::read_input_file()
{
    debug("read_input_file");
    // while(read_input_line())
    // {
    //     debug_vector(tokens_);
    // }
    bool arg_finish = false;
    if(read_input_line() && tokens_[tokens_.size()-1] == "generator")
    {
        // debug("first_line_read");
        while(!arg_finish && read_input_line() && tokens_[0][0] == '#')
        {
            for(string& token : tokens_)
            {
                // token = token.erase('#');
                // token.erase(remove(token.begin(), token.end(), '#'), token.end());
                // debug(token);
                token.erase(remove(token.begin(), token.end(), '#'), token.end());
                
                vector<string> tokens = get_tokens(token, "=");
                if(tokens[0] == "numio")
                {
                    numio_ = string2int(tokens[1]);
                    debug(numio_);
                }
                if(tokens[0] == "maxtracks")
                {
                    maxtracks_ = string2int(tokens[1]);
                    debug(maxtracks_);
                }
                if(tokens[0] == "lambda")
                {
                    lambda_ = string2float(tokens[1]);
                    debug(lambda_);
                    arg_finish = true;
                    break;
                }
                // debug_vector(tokens);
                // debug(token);
            }
            // get parameters from
            // debug("here");
            // debug_vector(tokens_);
            //continue the commend line
        }

    //     // debug_vector(tokens_);
    //     // process_count_ = string2int(tokens_[0]);
    }
}


bool InputHandler::read_instruction()
{
    if(read_input_line() && tokens_[0][0] != '#')
    {
        time_   = string2int(tokens_[0]);
        track_  = string2int(tokens_[1]);
        return true;
    }
    else
    {
        debug("read_end");
        return false;
    }
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


void InputHandler::read_all_instruction()
{
    int rid = 0;
    while(read_instruction())
    {
        IORequest io_request = IORequest(rid, time_, track_);
        io_request_queue.emplace_back(io_request);
        rid += 1;
    }
}

template<typename T>
T& createNullRef() { return *static_cast<T*>(nullptr); }


void InputHandler::simulate()
{
    bool finished = false;
    IORequest& cur_io_request = createNullRef<IORequest>();
    while(true)
    {
        // frontrequest = NULL;
        cur_io_request = createNullRef<IORequest>();
        if (io_request_queue.empty() == false)
        {
            cur_io_request = io_request_queue.front();
            if(cur_io_request.arrive_time_ == kTime)
            {
                p_io_scheduler->add_io_request(cur_io_request);
                io_request_queue.pop_front();
            }
        }
        if(&cur_io_request != nullptr)
        {
            
        }
        




        kTime += 1;
    }
    
    // while(read_instruction())
    // {
        
    // }
}

void InputHandler::summary()
{
    
}