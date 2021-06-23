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
#include "event_manager.hpp"
#include "utils.hpp"
// #include "process_manager.hpp"

using namespace std;
// InputHandler& g_input_handler;




// int get_random()
// {

// }
    



int main(int argc, char *argv[])
{
    InputHandler input_handler(argc, argv);
    InputHandler* p_input_handler = &input_handler;
    // g_input_handler = input_handler;
    int num = input_handler.arg_parse();
    if(argc != (num + 3)) // including the programs
    {
        cout<<"please input inputfile and randfile";
        return -1;
    }
    
    input_handler.create_process_from_input();
    

    // debug(event_manager.event_queue.size());
    // debug(event_manager->event_queue.empty());
    EventManager event_manager =  EventManager(p_input_handler ,input_handler.input_process_queue);

    //scheduler
    Scheduler *s = create_scheduler(input_handler.scheduler_type);
    event_manager.s = s;

    event_manager.Simulation();
//    Scheduler* s = new FCFSScheduler();
//    s->get_next_process();
//    cout<<s.scheduler_queue;
//    debug(s->scheduler_queue);
    // verbose = true;
    // debug(verbose);
    // Simulation(s, event_manager);
}

