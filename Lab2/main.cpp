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
// #include "process_manager.hpp"



using namespace std;


void Simulation(Scheduler* s, EventManager& event_manager) 
{
    Event* evt;
    // event_transition Next_Transition;
    Process* cur_process = NULL;
    int cur_time = 0;
    int cur_block_time = 0;
    bool call_scheduler = false;
    // int CURRENT_BLOCK_TIME = 0;
    while ((evt = event_manager.get_event())) 
    {
        debug(evt);
        Process* cur_p = evt->p;     // this is the process the event works on
        cur_time = evt->timestamp;  // current time
        int time_in_prev_state = cur_time - proc->state_ts;

        // event_transition Next_Transition;
        // Process* CURRENT_RUNNING_PROCESS = NULL;
        // bool CALL_SCHEDULER = false;
        // int CURRENT_TIME = 0;
        // int CURRENT_BLOCK_TIME = 0;

        //process event transition
        switch(evt->transition) 
        { 
            case TRANS_TO_READY:
            {
                // must come from BLOCKED or from PREEMPTION 
                // must add to run queue
                cur_p->state_ts = cur_time; //set process time
                Trace_Ready(proc, CURRENT_TIME, ih.verbose, flag1, flag2);

                // proc->state_ts = CURRENT_TIME;
            }
            case TRANS_TO_RUN:
            {

            }
            case TRANS_TO_BLOCK:
            {

            }
            case TRANS_TO_PREEMPT:
            {

            }
        }
        // post process
        delete evt; 
        evt = nullptr;

        // call scheduler
        if (call_scheduler) 
        {
            if (event_manager.get_next_event_time() == cur_time)
            {
                continue; //process next event from Event queue
            }
            call_scheduler = false; // reset global flag
            if (cur_process == nullptr) 
            {
                cur_process = s->get_next_process();
                if (cur_process == nullptr)
                    continue;
                Event* e = new Event(cur_process, cur_time, TRANS_TO_RUN);
                event_manager.put_event(e);
            }
        }
    }
    
}


int main(int argc, char *argv[])
{
    InputHandler input_handler(argc, argv);
    int num = input_handler.arg_parse();
    if(argc != (num + 3)) // including the programs
    {
        cout<<"please input inputfile and randfile";
        return -1;
    }
    input_handler.create_process_from_input();
    EventManager event_manager = EventManager(input_handler.input_process_queue);
    Scheduler* s = create_scheduler(input_handler.scheduler_type);
    Simulation(s, event_manager);
}

