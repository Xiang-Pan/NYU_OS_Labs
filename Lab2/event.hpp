#pragma once
#include <stdio.h>
#include <string.h>
#include "process.hpp"


// typedef enum {  } ;


//define transition here 

// enum Event_STATE
// {
//     state_ready,
//     state_run,
//     state_block,
//     state_preempt,
//     state_done
// };

enum Event_transition
{
    TRANS_TO_READY,
    TRANS_TO_RUN,
    TRANS_TO_BLOCK,
    TRANS_TO_PREEMPT,
    TRANS_TO_DONE
};



class Event
{
public:
    Process* p;
    int timestamp;
    Event_transition transition;
    Process_STATE old_state;
    Process_STATE new_state;

    bool operator == (const Process &p) 
    {
        return (this->p->pid == p.pid);
    }

    Event();

    Event(Process* process, int timestamp, Event_transition event_transition) {
        p = process;
        timestamp = timestamp;
        transition = event_transition;
    }
};
