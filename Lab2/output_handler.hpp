#pragma once
#include "process.hpp"
#include "event.hpp"
#include "utils.hpp"
using namespace std;

void log_transition(Process* p, Transition_type transition_type, int timestamp, int runtime = 0)
{
    // debug("log_transition");
    // debug(verbose);
    if(!verbose)
    {
        return;
    }
    switch (transition_type)
    {
        case created_ready:
        {
            printf("%d %d 0: CREATED -> READY\n", timestamp, p->pid);
            break;
        }
        case blocked_ready:
        {
            printf("%d %d %d: BLOCK -> READY\n", timestamp, p->pid, p->time_in_prev_state);
            break;
        }
        case ready_running:
        {
            printf("%d %d %d: READY -> RUNNG cb=%d rem=%d prio=%d\n", timestamp, p->pid, p->time_in_prev_state, p->remaining_CB+runtime, p->RC+runtime,p->dynamic_prio);
            break;
        }
        case running_blocked:
        {
            if(p->RC == 0)
            {
                printf("%d %d %d: Done\n", p->FT, p->pid, p->time_in_prev_state);
            }
            else
            {
                printf("%d %d %d: RUNNG -> BLOCK  ib=%d rem=%d\n", timestamp, p->pid, p->time_in_prev_state, runtime, p->RC);
            }
            break;
        }
        case running_preempt:
        {
            printf("%d %d %d: RUNNG -> READY  cb=%d rem=%d prio=%d\n", timestamp, p->pid, p->time_in_prev_state, p->remaining_CB, p->RC, p->dynamic_prio);
            break;
        }
        case preempt_running:
        {
            printf("%d %d %d: READY -> RUNNG cb=%d rem=%d prio=%d\n", timestamp, p->pid, p->time_in_prev_state, p->remaining_CB + runtime, p->RC + runtime, p->dynamic_prio);
            break;
        }
            
    }

    // if (p->created == 0)
    // {
    //     printf("%d %d 0: CREATED -> READY\n", timestamp, p->pid);
    //     p->created = 1;
    // }
    // else
    // {
    //     printf("%d %d %d: BLOCK -> READY\n", timestamp, p->pid, p->timeInPrevState);
    // }
}
// void Trace_Ready(Process* p, int timestamp, int verbose, int flag1, int flag2)
// {
//     if (verbose == 1)
//     {
//         if (flag1 == 0 && flag2 == 0)
//         {
//             if (p->created == 0)
//             {
//                 printf("%d %d 0: CREATED -> READY\n", timestamp, p->pid);
//                 p->created = 1;
//             }
//             else
//             {
//                 printf("%d %d %d: BLOCK -> READY\n", timestamp, p->pid, p->timeInPrevState);
//             }
//         }
//     }
// }