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
#include "scheduler.hpp"
#include "utils.hpp"
using namespace std;


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

        deque<Process*> input_process_queue;

        void create_process_from_input();

        int quantum=10000;
        int maxprio = 4;

        // random file 
        int get_random_num(int burst);
        int get_random_seed();
        void read_randomfile();
        Scheduler_type scheduler_type;

        // input file
        vector<string> tokens;
        bool read_inputfile();

        //read fill utils
        vector<string> get_tokens(string line_str, const string delim);


    private:
        ifstream inputfile_stream; // AC TC CB IO
        ifstream randomfile_stream;
        int total_random_count;
        int cur_random_count;
        int cur_random_seed;
        int cur_random_num;
};


InputHandler::InputHandler(int p_argc, char** p_argv)
{
    argc = p_argc;
    argv = p_argv;
    inputfile_stream.open((argv[argc-2])); // AC TC CB IO
    randomfile_stream.open((argv[argc-1]));
    // debug(argc);
    read_randomfile(); // have solved the first line
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

// TODO HERE
int InputHandler::get_random_seed()
{
    string line;
    if(!getline(randomfile_stream, line))
    {
        randomfile_stream.clear();                 // clear fail and eof bits
        randomfile_stream.seekg(0, std::ios::beg); // back to the start!
        getline(randomfile_stream, line);
    }
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


bool InputHandler::read_inputfile()
{
    string line;
    bool succ = static_cast<bool>(getline(inputfile_stream, line));
    if(succ)
    {
        tokens = get_tokens(line);
    }
    return succ;
}


int string2int(string s)
{
    stringstream ss(s);
    int x = 0;
    ss >> x;
    return x;
}


void InputHandler::create_process_from_input()
{
    int pid = 0;
    while(read_inputfile())
    {
        Process* p = new Process();
        p->pid = pid;
        p->AT = string2int(tokens[0]);
        p->TC = string2int(tokens[1]);
        p->CB = string2int(tokens[2]);
        p->IO = string2int(tokens[3]);
        
        p->RC = p->TC;

        p->static_prio = get_random_num(maxprio);
        p->dynamic_prio = p->static_prio - 1;
        input_process_queue.push_back(p);
        pid += 1;
    }
}

int InputHandler::arg_parse()
{
    int num = 0;
    int o = 0;
    const char *optstring = "vtes:"; 
    string a;
    string param;
    while ((o = getopt(argc, argv, optstring)) != -1) 
    {
        num += 1;
        switch (o)
        {
            case 'v':
                verbose = true;
                // printf("opt is v, oprarg is: %s\n", optarg);
                break;
            case 't':
                printf("opt is t, oprarg is: %s\n", optarg);
                break;
            case 'e':
                printf("opt is e, oprarg is: %s\n", optarg);
                break;
            case 's':
                // debug(optarg);
                // a = *optarg;
                a = optarg[0];
                // bool a = (*optarg == "F");
                // debug(a);
                if(a == "F")
                {
                    scheduler_type = FCFS;
                    break;
                }
                else if(a == "L")
                {
                    scheduler_type = LCFS;
                    break;
                }
                else if(a == "S")
                {
                    scheduler_type = SRTF;
                    break;
                }
                else if(a == "R")
                {
                    scheduler_type = RoundRobin;
                    param = optarg;
                    param = param.substr(1);
                    quantum = string2int(param);
                    break;
                }
                else if(a == "P")
                {
                    scheduler_type = PRIO;
                    sscanf(optarg, "%*c%d:%d", &quantum, &maxprio);
                    debug(quantum);
                    break;
                }
                else if(a == "E")
                {
                    scheduler_type = PREPRIO;
                    sscanf(optarg, "%*c%d:%d", &quantum, &maxprio);
                    debug(quantum);
                    break;
                }
                break;
            case '?':
                printf("error optopt: %c\n", optopt);
                printf("error opterr: %d\n", opterr);
                break;
        }
    }
    return num;
}
