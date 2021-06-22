#pragma once
#include <string>
#include <deque>
#include "process.hpp"

using namespace std;

enum Scheduler_type
{
    FCFS,
    LCFS
};

class Scheduler
{
    public:
        // Scheduler();
        deque<Process*> scheduler_queue;

        // default inplementation
        virtual void add_process(Process *p)
        {
            scheduler_queue.push_back(p);
            return;
        };


        virtual Process* get_next_process()
        {
            if(scheduler_queue.size() == 0)
                return NULL;
            Process* cur_p = scheduler_queue.front();
            scheduler_queue.pop_front();
            return cur_p;
        };

        void test_preempt(Process *p, int curtime); // typically NULL but for ‘E’
};


// First come first served sheduler.
class FCFSScheduler: public Scheduler
{
    public:
        FCFSScheduler();
};

FCFSScheduler::FCFSScheduler() 
{
    return;
}


Scheduler* create_scheduler(Scheduler_type scheduler_type)
{
    Scheduler* s;
    if(scheduler_type==FCFS)
    {
        s = new FCFSScheduler();
    }
    return s;

}


// class Scheduler
// {
// public:
    
//     virtual void add_Process(Process* pro) { return; };
//     virtual Process* get_next_Process() { Process* temp = new Process; return temp; };
// };



