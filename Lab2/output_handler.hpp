/*
 * @Author: Xiang Pan
 * @Date: 2021-06-21 14:00:56
 * @LastEditTime: 2021-07-13 16:53:45
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab2/output_handler.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include "process.hpp"
#include "event.hpp"
#include "utils.hpp"
using namespace std;

void log_preemption(Process* p, Process* cur_running_process, int timestamp)
{
    // ---> PRIO preemption 1 by 2 ? 0 TS=97 now=94) --> NO
    if(!verbose)
    {
        return;
    }
    bool is_preempt = p->dynamic_prio > cur_running_process->dynamic_prio && cur_running_process->next_time != timestamp;
    printf("---> PRIO preemption %d by %d ? %d TS=%d now=%d) ", cur_running_process->pid, p->pid, p->dynamic_prio > cur_running_process->dynamic_prio, cur_running_process->next_time, timestamp);
    if(is_preempt)
    {
        printf("--> YES\n");
    }
    else
    {
        printf("--> NO\n");
    } 
    return;
}


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
}
