#pragma once
#include "process.hpp"


using namespace std;

// class OutputHhandler
// {
//     public:
//         // void print(deque<Process*> Process_List, int Finish_Time, int IO_Time, string type);
//         // void Trace_Ready(Process* p, int timestamp, int verbose,int flag1,int flag2);
//         // void Trace_Run(Process* p, int timestamp, int verbose, int runtime);
//         // void Trace_Block(Process* p, int timestamp, int verbose,int IO_Time, int blocktime);
//         // void Trace_Preempt(Process* p, int timestamp, int verbose,string type);
//         // void Trace_Done(Process* p, int timestamp, int verbose);
//         // void Trace_Pre(Process* p, Process* curr, int timestamp, int verbose, int flag1, int flag2);
// };

void Trace_Ready(Process* p, int timestamp, int verbose, int flag1, int flag2)
{
    if (verbose == 1)
    {
        if (flag1 == 0 && flag2 == 0)
        {
            if (p->created == 0)
            {
                printf("%d %d 0: CREATED -> READY\n", timestamp, p->pid);
                p->created = 1;
            }
            else
            {
                printf("%d %d %d: BLOCK -> READY\n", timestamp, p->pid, p->timeInPrevState);
            }
        }
    }
}