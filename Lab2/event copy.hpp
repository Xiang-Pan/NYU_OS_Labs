#pragma once
#include <stdio.h>
#include <string.h>
#include "process.hpp"
#include "event_manager.hpp"
#include "output_handler.hpp"
#include "utils.hpp"

using namespace std;

// typedef enum {  } ;
class EventManager;

//define transition here 


// enum Transition_type
// {
//     created_ready,
//     ready_running,
//     running_ready,
//     running_blocked,
//     blocked_ready
// };

// enum Event_transition
// {
//     TRANS_TO_READY,
//     TRANS_TO_RUN,
//     TRANS_TO_BLOCK,
//     TRANS_TO_PREEMPT,
//     TRANS_TO_DONE
// };





class Event
{
public:
    Process* p;
    // InputHandler &g_input_handler;
    int timestamp;
    Event_transition transition;
    EventManager* event_manager;

    int next_time;
    Event_transition next_transition;
    
    Process_STATE old_state;
    Process_STATE new_state;
    Transition_type transition_type;
    InputHandler* ih;
    Scheduler* s;
    // input_process_queue
    // EventManager* event_manager;
    // EventManager event_manager;

    Event* next_event = nullptr;
    

    // int cur_time;

    bool operator == (const Process &p) 
    {
        return (this->p->pid == p.pid);
    }

    Event();

    // void Event::make_transition(Process_STATE pre_state, Process_STATE next_state);

    Event(Process* process, int p_timestamp, Event_transition event_transition)
    {
        p = process;
        old_state = p->state;
        timestamp = p_timestamp;
        transition = event_transition;
    }

    // void run(int cur_time);


    void get_new_state();
    void make_transition();
    void* transition_function;


    //transition functions
    void created_ready_fun();
    void blocked_ready_fun();
    void ready_running_fun();
    void running_blocked_fun();
    bool call_scheduler = true;
    // void created_ready_fun();
    // void created_ready_fun();
    // void create_ready();
    // void create_ready();
};

void Event::get_new_state()
{
    //set old state
    old_state = p->state;
    switch(transition) 
    { 
        case TRANS_TO_READY:
        {
            new_state = state_ready;
            call_scheduler = true;
            break;
        }
        case TRANS_TO_RUN:
        {
            new_state = state_running;
            break;
        }
        case TRANS_TO_BLOCK:
        {
            new_state = state_blocked;
            break;
        }
        case TRANS_TO_PREEMPT:
        {
            new_state = state_preempt;
            break;
        }
    }
    // debug(new_state);
}


void Event::make_transition()
{
    // void* f;
    // debug(old_state);
    // debug(new_state);
    // debug(transition);
    // debug(old_state == state_created && new_state == state_ready);
    if(old_state == state_created && new_state == state_ready)  
    {
        transition_type = created_ready;
        p->state = state_ready;
        created_ready_fun();
        s->add_process(p);
    }
    if(old_state == state_blocked && new_state == state_ready)  
    {
        transition_type = blocked_ready;
        blocked_ready_fun();
    }
    if(old_state == state_ready && new_state == state_running)  // TRANS_TO_RUN
    {
        transition_type = ready_running;
        ready_running_fun();
    }
    if(old_state == state_running && new_state == state_ready)
    {
        // debug("running_ready");
        // ready_running_fun();
        // running_ready_fun();
        transition_type = running_ready;
    }
    if(old_state == state_running && new_state == state_blocked)
    {
        transition_type = running_blocked;
        running_blocked_fun();
    }
    // if(old_state == state_blocked && new_state == state_blocked)
    // {
    //     transition_type = created_ready;
    // }
}

void Event::created_ready_fun()
{
    // void log_transition(Process* p, Transition_type transition_type, int timestamp)
    log_transition(p, transition_type, timestamp);
    // if (cur_process!= NULL)
    // {
    //     if (ih.preprio == 1 && proc->dynamic_Prio > CURRENT_RUNNING_PROCESS->dynamic_Prio)
    //     {
    //         flag1 = 1;
    //         if (CURRENT_RUNNING_PROCESS->nexttime - CURRENT_TIME > 0)
    //         {
    //             flag2 = 1;
    //             eh.remove_event(CURRENT_RUNNING_PROCESS);
    //             CURRENT_RUNNING_PROCESS->RC = CURRENT_RUNNING_PROCESS->RC + CURRENT_RUNNING_PROCESS->nexttime - CURRENT_TIME;
    //             CURRENT_RUNNING_PROCESS->remaining_CB = CURRENT_RUNNING_PROCESS->remaining_CB + CURRENT_RUNNING_PROCESS->nexttime - CURRENT_TIME;
    //             out.Trace_Pre(proc, CURRENT_RUNNING_PROCESS, CURRENT_TIME, ih.verbose, flag1, flag2);
    //             CURRENT_RUNNING_PROCESS->nexttime = CURRENT_TIME;
    //             Event* e = new Event(CURRENT_RUNNING_PROCESS, CURRENT_TIME, TRANS_TO_PREEMPT);
    //             eh.put_event(e);
                
    //         }
    //         else
    //         {
    //             out.Trace_Pre(proc, CURRENT_RUNNING_PROCESS, CURRENT_TIME, ih.verbose, flag1, flag2);
    //         }
    //     }
    //     else
    //     {
    //         if (ih.preprio == 1)
    //         {
    //             out.Trace_Pre(proc, CURRENT_RUNNING_PROCESS, CURRENT_TIME, ih.verbose, flag1, flag2);

    //         }
    //     }
    // }
}


void Event::blocked_ready_fun()
{

}

void Event::ready_running_fun()
{
    int runtime = 0;
    // // CURRENT_RUNNING_PROCESS = proc;
    p->CW += p->time_in_prev_state;
    p->state_ts = timestamp;  // cur_time
    // // if we do not have a remaining CPU burst then we generate one.
    if (p->remaining_CB == 0)
    {
        p->remaining_CB = ih->get_random_num(p->CB);
        p->generated_CB = p->remaining_CB;
        // ih->ofs++; //TODO
    }
    // // // if the remaining cpu burst in this time is less than or equal to the quantum
    // // // then the next time is block
    if (p->remaining_CB > p->RC)
    {
        p->remaining_CB = p->RC;
    }
    if (p->remaining_CB <= ih->quantum)
    {
        next_transition = TRANS_TO_BLOCK;
        runtime = p->remaining_CB;
        p->remaining_CB = 0;
    }
    // // //else which means the quantum of this process is all used, then it needs to be preempt.
    else
    {
        next_transition = TRANS_TO_PREEMPT;
        runtime = ih->quantum;
        p->remaining_CB -= ih->quantum;
    }
    if (p->RC - runtime < 0)
    {
        runtime = p->RC;
    }
    // // // remaining cpu time should be adjusted
    p->RC -= runtime;
    next_time = timestamp + runtime;
    p->state = state_running;
    log_transition(p, transition_type, timestamp, runtime);
    // out.Trace_Run(proc, CURRENT_TIME, ih.verbose,runtime);


    next_event = new Event(p, next_time, next_transition);
    
}

void Event::running_blocked_fun()
{
    int block_time = 0;
    p->state_ts = timestamp; // current event timestamp
    // if the remaining cpu time is 0 which means it is already done. Then we write done the finisth time and do nothing about I/O
    if (p->RC <= 0)
    {
        p->RC = 0;
        p->FT = timestamp;
        // out.Trace_Done(proc, proc->FT, ih.verbose);
    }
    // which means the process needs I/O
    else
    {
    //     block_time = ih->get_random_num(p->IO);
    //     p->state = state_blocked;
    //     p->IT += block_time;
    //     p->dynamic_prio = p->static_prio - 1;

    //     //set next event
    //     next_event= new Event(p, timestamp + block_time, TRANS_TO_READY);
    //     int CURRENT_BLOCK_TIME = 0;
    //     int IO_Time = 0;
    //     if (timestamp < CURRENT_BLOCK_TIME)
    //     {
    //         if (timestamp + block_time > CURRENT_BLOCK_TIME) {
    //             IO_Time = IO_Time + CURRENT_TIME + blocktime - CURRENT_BLOCK_TIME;
    //             CURRENT_BLOCK_TIME = CURRENT_TIME + blocktime;
    //         }
    //     }
    //     else {
    //         IO_Time = IO_Time + blocktime;
    //         CURRENT_BLOCK_TIME = CURRENT_TIME + blocktime;
    //     }
    //     // out.Trace_Block(proc, CURRENT_TIME, ih.verbose, IO_Time,blocktime);
    }
    // CURRENT_RUNNING_PROCESS = nullptr;
    call_scheduler = true;
    log_transition(p, transition_type, timestamp, block_time);

}


// void Event::run(int cur_time)
// {
    // int time_in_prev_state = cur_time - p->state_ts;
    // next_state = transition; 
    // switch(transition) 
    // { 

        // case TRANS_TO_READY:
        // {

        //     // must come from BLOCKED or from PREEMPTION 
        //     // must add to run queue
        //     // cur_p->state_ts = cur_time; //set process time
        //     // Trace_Ready(proc, CURRENT_TIME, ih.verbose, flag1, flag2);

        //     // proc->state_ts = CURRENT_TIME;
        // }
        // case TRANS_TO_RUN:
        // {

        // }
        // case TRANS_TO_BLOCK:
        // {

        // }
        // case TRANS_TO_PREEMPT:
        // {

        // }
    // }
    // post process
    // delete evt; 
    // evt = nullptr;

    // call scheduler
    // if (call_scheduler) 
    // {
    //     if (event_manager.get_next_event_time() == cur_time)
    //     {
    //         continue; //process next event from Event queue
    //     }
    //     call_scheduler = false; // reset global flag
    //     if (cur_process == nullptr) 
    //     {
    //         cur_process = s->get_next_process();
    //         if (cur_process == nullptr)
    //             continue;
    //         Event* e = new Event(cur_process, cur_time, TRANS_TO_RUN);
    //         event_manager.put_event(e);
    //     }
    // }
// }


// void Event::make_transition(Process_STATE pre_state, Process_STATE next_state)
// {

// }