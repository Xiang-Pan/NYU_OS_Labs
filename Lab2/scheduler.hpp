#pragma once
#include <string>
#include <deque>
#include "process.hpp"
#include "utils.hpp"
using namespace std;

enum Scheduler_type
{
    FCFS,
    LCFS,
    SRTF,
    RoundRobin,
    PRIO,
    PREPRIO,
    ABSTRACT
};

class Scheduler
{
    public:
        // Scheduler();
        deque<Process*> scheduler_queue;
        bool is_preemptible = false;

        // default inplementation
        virtual void add_process(Process *p)
        {
            scheduler_queue.push_back(p);
            return;
        };


        virtual Process* get_next_process()
        {
            // debug("base_class");
            if(scheduler_queue.size() == 0)
                return NULL;
            Process* cur_p = scheduler_queue.front();
            scheduler_queue.pop_front();
            return cur_p;
        };

        virtual Scheduler_type get_scheduler_type()
        {
            return ABSTRACT;
        }
        
        
        virtual string get_scheduler_name()
        {
            return "SCHED";
        }
        
        
        

        void test_preempt(Process *p, int curtime); // typically NULL but for ‘E’
};


// First come first served sheduler.
class FCFSScheduler: public Scheduler
{
    public:
        string scheduler_name = "FCFS";
        FCFSScheduler();
        virtual Scheduler_type get_scheduler_type()
        {
            return FCFS;
        }
        string get_scheduler_name()
        {
            return "FCFS";
        }
};

FCFSScheduler::FCFSScheduler() 
{
    return;
}


class LCFSScheduler: public Scheduler
{
    public:
        LCFSScheduler()
        {

        }
        virtual Scheduler_type get_scheduler_type()
        {
            return LCFS;
        }
        virtual string get_scheduler_name()
        {
            return "LCFS";
        }
        void add_process(Process* p)
        {
            scheduler_queue.push_front(p);
            return;
        }
};

//shortest remaining time first.
class SRTFScheduler: public Scheduler
{
    public:
        SRTFScheduler()
        {

        }
        virtual Scheduler_type get_scheduler_type()
        {
            return SRTF;
        }
        
        virtual string get_scheduler_name()
        {
            return "SRTF";
        }
        void add_process(Process* p)
        {
            if(scheduler_queue.empty())
            {
                scheduler_queue.push_back(p);
                return;
            }
            int remaining_time = p->RC;
            // auto itrFind = find_if(scheduler_queue.begin(), scheduler_queue.end(), [p] (int remaining_time) { return (p->RC < remaining_time); } );

            // auto itrFind = find_if(scheduler_queue.begin(), scheduler_queue.end(), [](Process* p, int remaining_time)
            // {
            //     return p->RC < remaining_time; 
            // });

            int i = 0;
            for (i; i < scheduler_queue.size(); i++)
            {
                if (scheduler_queue.at(i)->RC > remaining_time)
                    break;
            }
            std::deque<Process*>::iterator it = scheduler_queue.begin() + i;
            scheduler_queue.insert(it, p);
            return;
        }

};

class RoundRobinScheduler: public Scheduler
{
    public:
        RoundRobinScheduler(int p_quantum)
        {
            quantum = p_quantum;
        }
        int quantum= -1;
        // int preprio = 2;
        
        virtual Scheduler_type get_scheduler_type()
        {
            return RoundRobin;
        }
        virtual string get_scheduler_name()
        {
            string name = "RR ";
            name += to_string(quantum);
            return name;
        }
        void add_process(Process* p)
        {
            if (p->dynamic_prio == -1)
            {
                p->dynamic_prio = p->static_prio - 1;
            }
            scheduler_queue.push_back(p);
            return;
        }
};

class PRIOScheduler: public Scheduler
{
    public:
        int quantum = 10000;
        int maxprio = 4;
        deque<Process*>* activeQ;
        deque<Process*>* expiredQ;
        Scheduler_type scheduler_type;

        virtual Scheduler_type get_scheduler_type()
        {
            return scheduler_type;
        }
        

        void add_process(Process* p) override;
        Process* get_next_process() ;

        bool Empty(deque<Process*>* runQueue);

        virtual string get_scheduler_name()
        {
            string name;
            if(scheduler_type == PRIO)
            {
                name = "PRIO ";
            }
            else
            {
                name = "PREPRIO ";
            }
            name += to_string(quantum);
            return name;
        }

        PRIOScheduler(int param_quantum, int param_maxprio, Scheduler_type param_scheduler_type)
        {
            quantum = param_quantum;
            maxprio = param_maxprio;
            activeQ = new deque<Process*>[maxprio];
            expiredQ = new deque<Process*>[maxprio];
            is_preemptible = true;
            scheduler_type = param_scheduler_type;
        }
};


bool PRIOScheduler::Empty(deque<Process*> *runqueue)
{
    for (int i = 0; i < maxprio; i++)
    {
        if (runqueue[i].size() != 0)
        {
            return false;
        }
    }
    return true;
}

void PRIOScheduler::add_process(Process* p)
{
    // default value
    // debug(p->dynamic_prio);
    if (p->dynamic_prio == -1)
    {
        p->dynamic_prio = p->static_prio - 1;
        expiredQ[p->dynamic_prio].push_back(p);
        return;
    }
    activeQ[p->dynamic_prio].push_back(p);
    return;
}

Process* PRIOScheduler::get_next_process()
{
    // debug("PRIOScheduler::get_next_Process");

    if(Empty(activeQ) && Empty(expiredQ))
    {
        // debug("activeQ");
        return nullptr;
    }
    if (Empty(activeQ))
    {
        for (int i = 0; i < maxprio; i++)
        {
            int size = expiredQ[i].size();
            for (int j = 0; j < size; j++)
            {
                activeQ[i].push_back(expiredQ[i].front());
                expiredQ[i].pop_front();
            }
        }
    }
    int index = maxprio-1;
    for (index; index >= 0; index--)
    {
        if (activeQ[index].size() == 0)
        {
            continue;
        }
        else
        {
            break;
        }
    }
    Process* curr = activeQ[index].front();
    activeQ[index].pop_front();
    return curr;
}




Scheduler* create_scheduler(Scheduler_type scheduler_type, int quantum = 10000, int maxprio = 4)
{
    debug(scheduler_type);
    Scheduler* s;
    if(scheduler_type == FCFS)
    {
        s = new FCFSScheduler();
    }
    else if(scheduler_type == LCFS)
    {
        
        s = new LCFSScheduler();
    }
    else if(scheduler_type == SRTF)
    {
        s = new SRTFScheduler();
    }
    else if(scheduler_type == RoundRobin)
    {
        s = new RoundRobinScheduler(quantum);
        
    }
    else if(scheduler_type == PRIO)
    {
        s = new PRIOScheduler(quantum, maxprio, PRIO);
    }
    else if(scheduler_type == PREPRIO)
    {
        s = new PRIOScheduler(quantum, maxprio, PREPRIO);
    }
    s->get_scheduler_name();
    debug(s->get_scheduler_name());
    return s;
}





// class Scheduler
// {
// public:
    
//     virtual void add_Process(Process* pro) { return; };
//     virtual Process* get_next_Process() { Process* temp = new Process; return temp; };
// };



