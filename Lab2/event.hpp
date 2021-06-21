#pragma once
#include <stdio.h>
#include <string.h>
#include "process.hpp"


// typedef enum {  } event_transition;


//define transition here 

// enum Event_STATE
// {
//     state_ready,
//     state_run,
//     state_block,
//     state_preempt,
//     state_done
// };



class Event
{
public:
    Process* p;
    int timestamp;
    // Event_STATE event_state = state_ready;
    // event transition;

    // Event(Process* process, int ts, event_transition et) {
    //     p = process;
    //     timestamp = ts;
    //     // transition = et;
    // }
};
