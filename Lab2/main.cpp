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
InputHandler& g_input_handler;


void Simulation(Scheduler* s, EventManager& event_manager)
{
    Event* evt;
    // event_transition Next_Transition;
    Process* cur_running_process = NULL;
    int cur_time = 0;
    int cur_block_time = 0;
    bool call_scheduler = false;
    // debug(e);
    // int CURRENT_BLOCK_TIME = 0;
    while(evt = event_manager.get_event())
    {
        evt.input_handler = g_input_handler;
        // cur_running_process = evt->p; // the process works on
//        cur_time
        cur_time = evt->timestamp;
//        evt->cur_time = cur_time;
        evt->get_new_state();
        evt->make_transition();
        call_scheduler = evt->call_scheduler;
        s->add_process(evt->p);
        delete evt; 
        evt = nullptr;
        
        if(call_scheduler) 
        {
            if (event_manager.get_next_event_time() == cur_time)
                continue;           //process next event from Event queue
            call_scheduler = false; // reset call scheduler
            if (cur_running_process == nullptr) 
            {
                cur_running_process = s->get_next_process();
                if (cur_running_process == nullptr)
                    continue;
                Event* e = new Event(cur_running_process, cur_time, TRANS_TO_RUN);
                event_manager.put_event(e);

                
                // debug(event_manager.event_queue)
                // create event to make this process runnable for same time.
            }
        }
    }
    
    // int 


    
}
    



int main(int argc, char *argv[])
{
    InputHandler input_handler(argc, argv);
    // g_input_handler = input_handler;
    int num = input_handler.arg_parse();
    if(argc != (num + 3)) // including the programs
    {
        cout<<"please input inputfile and randfile";
        return -1;
    }
    input_handler.create_process_from_input();
    EventManager event_manager =  EventManager(input_handler.input_process_queue);
    // debug(event_manager.event_queue.size());
    // debug(event_manager->event_queue.empty());
    Scheduler *s = create_scheduler(input_handler.scheduler_type);
//    Scheduler* s = new FCFSScheduler();
//    s->get_next_process();
//    cout<<s.scheduler_queue;
//    debug(s->scheduler_queue);
    // verbose = true;
    // debug(verbose);
    Simulation(s, event_manager);
}

