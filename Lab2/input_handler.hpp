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
#include "process.hpp"
using namespace std;

#define VNAME(name) (#name)
#define debugging_enabled true
#define debug(x) do { \
  if (debugging_enabled) { std::cerr << VNAME(x)<<":"<< x << std::endl; } \
} while (0)

#define debug_vector(v) do { \
    if (debugging_enabled) {\
        for(int i=0; i<v.size(); i++)\
        {\
            std::cerr << i <<":"<< v[i] << std::endl;\
        }\
    } \
} while (0)


// INITIALIZE_EASYLOGGINGPP

void Simulation() 
{
    return;
}
    // while(evt = get_event())
    // {
        
    // }
// EVENT* evt; while( (evt = get_event()) ) { Process *proc = evt->evtProcess; // this is the process the event works on CURRENT_TIME = evt->evtTimeStamp; timeInPrevState = CURRENT_TIME – proc->state_ts;

// switch(evt->transition) { // which state to transition to?

// case TRANS_TO_READY:

// // must come from BLOCKED or from PREEMPTION // must add to run queue CALL_SCHEDULER = true; // conditional on whether something is run break; case TRANS_TO_RUN:

// // create event for either preemption or blocking break; case TRANS_TO_BLOCK:

// //create an event for when process becomes READY again CALL_SCHEDULER = true; break; case TRANS_TO_PREEMPT:

// // add to runqueue (no event is generated) CALL_SCHEDULER = true; break; } // remove current event object from Memory delete evt; evt = nullptr;

// if(CALL_SCHEDULER) {

// if (get_next_event_time() == CURRENT_TIME) continue; //process next event from Event queue CALL_SCHEDULER = false; // reset global flag if (CURRENT_RUNNING_PROCESS == nullptr) {

// CURRENT_RUNNING_PROCESS = THE_SCHEDULER->get_next_process();

// if (CURRENT_RUNNING_PROCESS == nullptr) continue;

// }

// }
int quantum = -1;
int maxproi = -1;


// create event to make this process runnable for same time. }


// int myrandom(int burst) 
// { 
//     return 1 + (randvals[ofs] % burst); 
// }

class InputHandler
{
    public:
        int argc;
        char ** argv;
        InputHandler(int argc, char** argv);
        int arg_parse();
        

        // random file 
        int get_random_num(int burst);
        int get_random_seed();
        void read_randomfile();

        // input file
        vector<string> tokens;
        bool read_inputfile();

        vector<string> get_tokens(string line_str, const string delim);





    private:
        ifstream inputfile_stream; // AC TC CB IO
        ifstream randomfile_stream;
        int total_random_count;
        int cur_random_count;
        int cur_random_seed;
        int cur_random_num;
        

;
    
};


InputHandler::InputHandler(int p_argc, char** p_argv)
{
    argc = p_argc;
    argv = p_argv;

    
    inputfile_stream.open((argv[argc-2])); // AC TC CB IO
    randomfile_stream.open((argv[argc-1]));
    // debug(argc);
    read_randomfile();
    // debug(argv[0]);
    
}

void InputHandler::read_randomfile()
{
    string line;
    getline(randomfile_stream, line);
    stringstream ss(line);
    ss >> total_random_count;
}


// int myrandom(int burst) 
// { 
//     return 1 + (randvals[ofs] % burst); 
// }

// int
int InputHandler::get_random_seed()
{
    string line;
    getline(randomfile_stream, line);
    stringstream ss(line);
    ss >> cur_random_seed;
    return cur_random_seed;
}


int InputHandler::get_random_num(int burst)
{
    get_random_seed();
    return 1 + (cur_random_seed % burst);;
}


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

int string2int(string s)
{
    // object from the class stringstream
    stringstream ss(s);
    int x = 0;
    ss >> x;
    return x;
}

bool InputHandler::read_inputfile()
{
    // inputfile_stream.getline
    string line;
    int pid = 0;
    bool succ = static_cast<bool>(getline(inputfile_stream, line));
    if(succ)
    {
        tokens = get_tokens(line);
    }
    return succ;
}


void create_process_from_input(InputHandler& input_handler,deque<Process*>* ProcessList)
{
    int pid = 0;
    vector<string> tokens;
    while(input_handler.read_inputfile())
    {
        tokens = input_handler.tokens;
        Process* p = new Process();
        p->pid = pid;
        p->AT = string2int(tokens[0]);
        p->TC = string2int(tokens[1]);
        p->CB = string2int(tokens[2]);
        p->IO = string2int(tokens[3]);
        p->RC = p->TC;
        pid += 1;
    }
}




int InputHandler::arg_parse()
{
    int num = 0;
    int o = 0;
    const char *optstring = "vtes:"; 
    while ((o = getopt(argc, argv, optstring)) != -1) 
    {
        num += 1;
        switch (o)
        {
            case 'v':
                printf("opt is v, oprarg is: %s\n", optarg);
                break;
            case 't':
                printf("opt is t, oprarg is: %s\n", optarg);
                break;
            case 'e':
                printf("opt is e, oprarg is: %s\n", optarg);
                break;
            case 's':
                printf("opt is s, oprarg is: %s\n", optarg);
                sscanf(optarg, "%d:%d", &quantum, &maxproi);
                // cout<<quantum<<maxproi;
                break;
            case '?':
                printf("error optopt: %c\n", optopt);
                printf("error opterr: %d\n", opterr);
                break;
        }
    }
    return num;
}
