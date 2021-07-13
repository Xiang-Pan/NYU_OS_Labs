/*
 * @Author: Xiang Pan
 * @Date: 2021-06-15 18:45:29
 * @LastEditTime: 2021-07-13 16:54:03
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab2/main.cpp
 * xiangpan@nyu.edu
 */
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <istream>
#include <vector> 
#include <regex>
#include  <unistd.h> //parse args
// #include "des.hpp"
#include "scheduler.hpp"
#include "input_handler.hpp"
#include "event_manager.hpp"
#include "utils.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    InputHandler input_handler(argc, argv);
    InputHandler* p_input_handler = &input_handler;
    int num = input_handler.arg_parse();
    if(argc != (num + 3)) // including the programs
    {
        cout<<"please input inputfile and randfile";
        return -1;
    }
    
    input_handler.create_process_from_input();
    
    // debug(event_manager.event_queue.size());
    // debug(event_manager->event_queue.empty());
    EventManager event_manager =  EventManager(p_input_handler);

    //scheduler
    debug(input_handler.scheduler_type);
    debug(input_handler.quantum);
    Scheduler *s = create_scheduler(input_handler.scheduler_type, input_handler.quantum, input_handler.maxprio);
    event_manager.s = s;

    event_manager.simulation();
    event_manager.summary();
}

