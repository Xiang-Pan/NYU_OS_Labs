/*
 * @Author: Xiang Pan
 * @Date: 2021-06-20 17:40:15
 * @LastEditTime: 2021-07-13 16:57:51
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab2/event.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include <stdio.h>
#include <string.h>
#include "process.hpp"
#include "event_manager.hpp"
#include "output_handler.hpp"
#include "utils.hpp"

using namespace std;

class Event
{
public:

    int timestamp;
    Event_transition transition;
    Transition_type transition_type;

    //process status
    Process* p;
    int cur_block_time;
    Process_STATE old_state;
    Process_STATE new_state;
    
    //cur running process information
    Process* cur_running_process;

    //manager info
    InputHandler* ih;
    Scheduler* s;

    Event* next_event = nullptr;
    int io_time = 0;


    bool operator == (const Process &p) 
    {
        return (this->p->pid == p.pid);
    }

    // Event();

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
    void running_preempt_fun();
    void preempt_running_fun();
    bool call_scheduler = false;
    bool preempt_cur_process = false;



    void test_preempt();
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
    p->time_in_prev_state = timestamp - p->state_ts;
    p->state_ts = timestamp; // set state timestamp
    
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
        p->state = state_ready;
        blocked_ready_fun();
        s->add_process(p);
    }
    if(old_state == state_ready && new_state == state_running)  // TRANS_TO_RUN
    {
        transition_type = ready_running;
        ready_running_fun();
    }
    // if(old_state == state_running && new_state == state_ready)
    // {
    //     // debug("running_ready");
    //     // ready_running_fun();
    //     // running_ready_fun();
    //     transition_type = running_ready;
    //     debug("running_ready");
    //     // running_ready_fun();
    // }
    if(old_state == state_running && new_state == state_blocked)
    {
        transition_type = running_blocked;
        running_blocked_fun();
    }
    if(old_state == state_running && new_state == state_preempt)
    {
        transition_type = running_preempt;
        running_preempt_fun();
    }
    if(old_state == state_preempt && new_state == state_running)
    {
        transition_type = preempt_running;
        preempt_running_fun();
    }
    // if(old_state == state_blocked && new_state == state_blocked)
    // {
    //     transition_type = created_ready;
    // }
}




void Event::test_preempt()
{
    if (s->get_scheduler_type() != PREPRIO)
    {
        return;
    }
    
    // try to preemet
    if (cur_running_process!= NULL)
    {
        log_preemption(p, cur_running_process, timestamp);
        // if(p->dynamic_prio > cur_running_process->dynamic_prio && cur_running_process->next_time != timestamp) 
        if(p->dynamic_prio > cur_running_process->dynamic_prio && cur_running_process->next_time != timestamp) 
        {
            //! begin preemtpt
            // delete cur_running_process feature event
            preempt_cur_process = true;
            // event_manager->remove_event(p);

            //calculate the remaing cpu time
            int delta_time = cur_running_process->next_time - timestamp;
            cur_running_process->RC = cur_running_process->RC + delta_time;
            cur_running_process->remaining_CB = cur_running_process->remaining_CB + delta_time; 

            //set to preemtpt state
            cur_running_process->next_time = timestamp;
            cur_running_process->next_transition = TRANS_TO_PREEMPT;
            next_event = new Event(cur_running_process, cur_running_process->next_time, cur_running_process->next_transition);
        }
    }
}


void Event::preempt_running_fun()
{
    ready_running_fun();
}


void Event::running_preempt_fun()
{
    cur_running_process = nullptr;

    
    // debug(s->is_preemptible);
    log_transition(p, transition_type, timestamp);
    if(s->is_preemptible) 
    {
        // debug("is_preemptible");
        p->dynamic_prio -= 1;
    }



    p->state = state_preempt;
    
    s->add_process(p);

    // add to runqueue (no event is generated)
    call_scheduler = true;

}



void Event::created_ready_fun()
{
    // void log_transition(Process* p, Transition_type transition_type, int timestamp)
    log_transition(p, transition_type, timestamp);
    test_preempt();
    // debug(cur_running_process == NULL);
}


void Event::blocked_ready_fun()
{
     // set p state time
    log_transition(p, transition_type, timestamp);
    test_preempt();


}

void Event::ready_running_fun()
{
    int runtime = 0;
    p->CW += p->time_in_prev_state;
    if (p->remaining_CB == 0)
    {
        p->remaining_CB = ih->get_random_num(p->CB);
        // debug(p->remaining_CB);
        p->generated_CB = p->remaining_CB;
        //! SOLVED reading
    }

    // quantum remaining, block
    if (p->remaining_CB > p->RC)
    {
        p->remaining_CB = p->RC;
    }

    if (p->remaining_CB <= ih->quantum)
    {
        p->next_transition = TRANS_TO_BLOCK;
        runtime = p->remaining_CB;
        p->remaining_CB = 0;
    }
    else // quantum of this process is all used, be preempt.
    {
        p->next_transition = TRANS_TO_PREEMPT;
        runtime = ih->quantum;
        p->remaining_CB -= ih->quantum;
    }

    if (p->RC - runtime < 0)
    {
        runtime = p->RC;
    }
    // remaining cpu time should be adjusted
    p->RC -= runtime;
    p->next_time = timestamp + runtime;
    p->state = state_running;
    
    log_transition(p, transition_type, timestamp, runtime);

    next_event = new Event(p, p->next_time, p->next_transition);
}

void Event::running_blocked_fun()
{
    int block_time = 0;
    
    if (p->RC <= 0) // done
    {
        p->RC = 0;
        p->FT = timestamp;
    }
    else            // which means the process needs I/O
    {
        block_time = ih->get_random_num(p->IO);
        p->state = state_blocked;
        p->IT += block_time;
        p->dynamic_prio = p->static_prio - 1;

        //set next event
        p->next_time = timestamp + block_time;
        p->next_transition = TRANS_TO_READY;
        next_event= new Event(p, p->next_time, p->next_transition);

        if (timestamp < cur_block_time)
        {
            if (timestamp + block_time > cur_block_time) 
            {
                io_time =  timestamp + block_time - cur_block_time;
                cur_block_time = timestamp + block_time;
            }
        }
        else 
        {
            io_time = block_time;
            cur_block_time = timestamp + block_time;
        }
    }

    cur_running_process = nullptr;
    call_scheduler = true;
    log_transition(p, transition_type, timestamp, block_time);
}

