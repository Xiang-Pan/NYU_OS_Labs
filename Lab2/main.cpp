#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <istream>
#include <vector> 
#include <regex>
#include  <unistd.h> //parse args
// #include "cmdline.h"
#include "des.hpp"
#include "scheduler.hpp"
#include "input_handler.hpp"
#include "process_manager.hpp"


using namespace std;


// void InputHandler::Read_Input_File(ifstream& inputfile, deque<Process*>* ProcessList)
// {
//     string line;
//     const char* delim = " '\t''\n'";
//     int pid = 0;
//     int processInfo[4];
//     while (getline(inputfile, line))
//     {
//         int i = 0;
//         char* dup;
//         char* token;
//         dup = strdup(line.c_str());
//         token = strtok(dup, delim);
//         while (token != NULL) {
//             processInfo[i] = atoi(token);
//             i++;
//             token = strtok(NULL, delim);
//         }
//         Process* p = new Process();
//         p->pid = pid;
//         p->AT = processInfo[0];
//         p->TC = processInfo[1];
//         p->CB = processInfo[2];
//         p->IO = processInfo[3];
//         p->RC = p->TC;
//         p->static_Prio = myrandom(maxprio);
//         p->dynamic_Prio = p->static_Prio - 1;
//         p->state_ts = p->AT;
//         ProcessList->push_back(p);
//         free(dup);
//         pid++;
//         ofs++;
//     }
// }


int main(int argc, char *argv[])
{
    InputHandler input_handler(argc, argv);



    int num = input_handler.arg_parse();
    if(argc != (num + 3)) // including the programs
    {
        cout<<"please input inputfile and randfile";
        return -1;
    }
    // input_handler.read_inputfile();


    ProcessManager process_manager;
    process_manager.create_process_from_input(input_handler);
    // fo
    // int anum = 10;
    // for(int i = 0; i < anum; i++)
    // {
    //     int t = input_handler.get_random_num();
    //     debug(t);
    // }
    
    Scheduler s = Scheduler();


}

