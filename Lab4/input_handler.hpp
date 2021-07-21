/*
 * @Author: Xiang Pan
 * @Date: 2021-07-14 02:04:44
 * @LastEditTime: 2021-07-21 05:05:49
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
#include <unistd.h>
#include <iomanip>      // Header file needed to use setw
#include "io_request.hpp"
#include "io_scheduler.hpp"
#include "output_handler.hpp"
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
            // debug(num);
            // debug(argc);
            if((num+2) != argc)
            {
                cout << "please input file name!" << endl;
            }
            inputfile_stream.open((argv[argc-1])); //
            read_input_file();
            read_all_instruction();
        }
        
        vector<string> get_tokens(string line_str,const std::string delim);


        int numio_;
        int maxtracks_;
        float lambda_;


        int time_;
        int track_;



        // int cur_track_;
        // bool cur_direction_;


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
    // debug(line);
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
    const char *optstring = "s:vq";
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
                    case 'j': 
                    {
                        p_io_scheduler = new SSTFIOScheduler();
                        break;
                    }
                    case 's': 
                    {
                        p_io_scheduler = new LOOKIOScheduler();
                        break;             
                    }
                    case 'c': 
                    {
                        p_io_scheduler = new CLOOKIOScheduler();
                        break;
                    }
                    case 'f': 
                    {
                        p_io_scheduler = new FLOOKIOScheduler();
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case 'v': 
            {
                kIsTrace = true;
                break;
            }
            case 'q': 
            {
                kDebugQueue = true;
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
        IORequest* p_io_request = new IORequest(rid, time_, track_);
        p_io_scheduler->all_requests_queue_.push(p_io_request);
        rid += 1;
    }
    requests_count = rid;
    // debug(requests_count);
}

void InputHandler::simulate()
{
    bool cur_finished = false;
    if(kIsTrace)
    {
        cout << "TRACE" << endl;
    }
    
    IORequest* p_cur_io_request = nullptr;
    IORequest* p_next_io_request = nullptr;
    while(true)
    {
        // debug(kTime);
        p_next_io_request = nullptr;
        // if a new I/O arrived to the system at this current time → add request to IO-queue
        if(p_io_scheduler->all_requests_queue_.empty() == false)  //get_next_io_request
        {
            p_next_io_request = p_io_scheduler->all_requests_queue_.front();
            if(p_next_io_request->arrive_time_ == kTime)     //if next io request is cur time
            {
                p_io_scheduler->add_io_request(p_next_io_request);
                p_io_scheduler->arrived_requests_vec_.emplace_back(p_next_io_request);
                p_io_scheduler->all_requests_queue_.pop();
                log_add(p_next_io_request);
            }
        }

        // if an IO is active 
        if(p_cur_io_request != nullptr)                     // p_cur_io_request first
        {
            // and completed at this time → Compute relevant info and store in IO request for final summary
            if (p_cur_io_request->track_ == kCurTrack)
            {
                p_cur_io_request->end_time_ = kTime;
                
                int wait_time = p_cur_io_request->start_time_ - p_cur_io_request->arrive_time_;
                if (wait_time > max_waittime)
                {
                    max_waittime = wait_time;
                }
                total_waittime += wait_time;
                int turn_aroundtime = p_cur_io_request->end_time_ - p_cur_io_request->arrive_time_;
                total_turnaround_time += turn_aroundtime;
                log_finish(p_cur_io_request);
                p_cur_io_request = nullptr;
                cur_finished = true;
            }
            else  // get the io
            {
                int target_track = p_cur_io_request->track_;
                kCurDirection = (target_track > kCurTrack) ? 1 : -1;
                kCurTrack += kCurDirection;
                total_movement += 1;
            }
        }

        // if no IO request active now if requests are pending  Fetch the next request from IO-queue and start the new IO.
        if (p_cur_io_request == nullptr && p_io_scheduler->empty() == false) // can get next
        {
            //fetch next io request from active_queue_
            p_cur_io_request = p_io_scheduler->get_next_io_request();
            // p_cur_io_request = p_io_scheduler->active_queue_.front();
            // p_io_scheduler->active_queue_.pop_front();

            if(p_cur_io_request->start_time_ == -1)
            {
                p_cur_io_request->start_time_ = kTime;
                cur_finished = false;    // reset cur finished
                log_issue(p_cur_io_request);
                continue;
            }
        }
        if (p_cur_io_request == nullptr && p_io_scheduler->empty() == true && p_io_scheduler->all_requests_queue_.empty() == true) // all empty
        {
            break;
        }
        kTime += 1;
    }
}

void InputHandler::summary()
{
    total_time = kTime;
    for (size_t i = 0; i < p_io_scheduler->arrived_requests_vec_.size(); i++) 
    {
        IORequest* r = (p_io_scheduler->arrived_requests_vec_)[i];
        cout << setw(5) << i << ": " 
        << setw(5) << r->arrive_time_ << " "
        << setw(5) << r->start_time_ << " "
        << setw(5) << r->end_time_ << endl;
    }
    avg_turnaround = (double)total_turnaround_time / requests_count;
	avg_waittime = (double)total_waittime / requests_count;
    printf("SUM: %d %d %.2lf %.2lf %d\n", total_time, total_movement, avg_turnaround, avg_waittime,  max_waittime);
}


// void OutputHandler::PrintIORequest()
// {
// 	int waittime = 0;
// 	int turnaroundtime = 0;
// 	while (!InputIOqueue.empty())
// 	{
// 		IORequest* iorequest = InputIOqueue.front();
// 		InputIOqueue.pop_front();
// 		printf("%5d: %5d %5d %5d\n", iorequest->rid, iorequest->arrive_time, iorequest->start_time, iorequest->end_time);
// 		waittime = iorequest->start_time - iorequest->arrive_time;
// 		turnaroundtime = iorequest->end_time - iorequest->arrive_time;
// 		if (waittime > max_waittime)
// 		{
// 			max_waittime = waittime;
// 		}
// 		total_waittime += waittime;
// 		total_turnaround_time += turnaroundtime;
// 	}
// 	avg_turnaround = (double)total_turnaround_time / requestnum;
// 	avg_waittime = (double)total_waittime / requestnum;
	
	
// }