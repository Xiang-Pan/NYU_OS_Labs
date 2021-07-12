#pragma once
#include <string>
#include <vector>
#include "event.hpp"
using namespace std;


class EventManager
{
    public:
        EventManager();
        EventManager(InputHandler* input_handler);
        deque<Event*> event_queue;  // des layer queue
        void set_event_queue(deque<Process*> input_process_queue);
        InputHandler* input_handler;

        // void Initiliaze_Event_Queue(deque<Process*> ProcessList);

        Event* get_event();
        void put_event(Event* e);
        void rm_event(Process* p);
        int get_next_event_time();
        Scheduler* s;

        Event * cur_event;
        Process * cur_running_process;
        // void Simulation();


        int io_time = 0;
        int finish_time = 0;
        void simulation();
        void summary();

};

EventManager::EventManager(InputHandler* p_input_handler)
{
    input_handler = p_input_handler;
    set_event_queue(p_input_handler->input_process_queue);
}


void EventManager::set_event_queue(deque<Process*> input_process_queue) 
{
    if(!event_queue.empty())
    {
        debug("event not empty");
    }
    for (int i = 0; i < input_process_queue.size(); i++)
    {
        Event* e = new Event(input_process_queue[i], input_process_queue[i]->AT, TRANS_TO_READY);
        // e->event_manager = this;
        event_queue.push_back(e);
    }
}

EventManager::EventManager()
{

}


Event* EventManager::get_event()
{
    // debug
    if(event_queue.empty())
    {
        debug("event_queue.empty");
        Event* pe = nullptr;
        return pe;
    }   
    Event* event = event_queue.front();
    event_queue.pop_front();
    // debug(event);
    cur_event = event;
    event->ih = input_handler;
    event->s = s;
    event->event_manager = this;
    return event;
}


void EventManager::put_event(Event* e)
{
    // e->event_manager = this;
    int i = 0;
    for (; i < event_queue.size(); i++)
    {
        if (event_queue.at(i)->timestamp > e->timestamp)
            break;
    }
    std::deque<Event*>::iterator it = event_queue.begin() + i;
    event_queue.insert(it, e);
}

void EventManager::rm_event(Process* p)
{
    int i = 0;
    for(; i < event_queue.size(); i++)
    {
        if (event_queue.at(i)->p->pid == p->pid)
            break;
    }
    event_queue.erase(event_queue.begin() + i);
}

int EventManager::get_next_event_time()
{
    if(event_queue.size() <= 0)
    {
        return -1;
    }
    else
    {
        return event_queue.front()->timestamp;
    }
}


void EventManager::simulation()
{
    Event* evt;
    // event_transition Next_Transition;
    Process* cur_running_process = NULL;
    int cur_time = 0;
    int cur_block_time = 0;
    bool call_scheduler = true;
    // int CURRENT_BLOCK_TIME = 0;

    // debug(e);
    // int CURRENT_BLOCK_TIME = 0;
    while(evt = get_event())
    {
        
        // evt.input_handler = g_input_handler;
        // cur_running_process = evt->p; // the process works on
//        cur_time
        cur_time = evt->timestamp;
//        evt->cur_time = cur_time;

        evt->cur_running_process = cur_running_process;
        evt->cur_block_time = cur_block_time;
        
        // set event transition
        evt->get_new_state();
        evt->make_transition();

        cur_block_time = evt->cur_block_time;
        cur_running_process = evt-> cur_running_process;

        //set next event based on cur process
        Event* next_event = evt->next_event;
        if(next_event != nullptr)
        {
            put_event(evt->next_event);
        }


        call_scheduler = evt->call_scheduler;
        io_time += evt->io_time;
        

        delete evt; 
        evt = nullptr;
        


        if(call_scheduler) 
        {
            debug("herer");
            debug(get_next_event_time());
            if (get_next_event_time() == cur_time)
                continue;           //process next event from Event queue
            call_scheduler = false; // reset call scheduler
            
            if (cur_running_process == nullptr) 
            {
                // debug(cur_running_process);
                // cout<< cur_running_process;
                cur_running_process = s->get_next_process();
                // debug(cur_running_process->pid);

                if (cur_running_process == nullptr)
                    continue;
                
                debug("come");

                // create event to make this process runnable for same time.
                Event* e = new Event(cur_running_process, cur_time, TRANS_TO_RUN);
                put_event(e);
                
            }
        }
    }
    finish_time = cur_time;
    
    
    
    // int 


    
}


void EventManager::summary()
{
    double Total_Turn = 0;
    double Total_CPUW = 0;
    double Total_CPU_T = 0;
    int process_num = input_handler->input_process_queue.size();
    cout << s->get_scheduler_name() << endl;
    for(int i = 0; i < input_handler->input_process_queue.size(); i++)
    {
        Process* tp = input_handler->input_process_queue.at(i);
        Total_Turn += tp->FT - tp->AT;
        Total_CPUW += tp->CW;
        Total_CPU_T += tp->TC;
        printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
            tp->pid,
            tp->AT,
            tp->TC,
            tp->CB,
            tp->IO,
            tp->static_prio,
            tp->FT,
            tp->FT - tp->AT,
            tp->IT,
            tp->CW);
    }
    double CPU_U = 100 * (double)Total_CPU_T / (double)finish_time;
    double IO_U = 100 * (double)io_time / (double)finish_time;
    double Average_TurnA = (double)Total_Turn / (double)process_num;
    double Average_CPUW = (double)Total_CPUW / (double)process_num;
    double Through_P = (double)process_num * 100 / (double)finish_time;

    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n", finish_time,
        CPU_U,
        IO_U,
        Average_TurnA,
        Average_CPUW,
        Through_P);
    
    // double CPU_U = 100 * (double)Total_CPU_T / (double)Finish_Time;
    // double IO_U = 100 * (double)IO_Time / (double)Finish_Time;
    // double Average_TurnA = (double)Total_Turn / (double)Process_List.size();
    // double Average_CPUW = (double)Total_CPUW / (double)Process_List.size();
    // double Through_P = (double)Process_List.size() * 100 / (double)Finish_Time;

    // printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n", Finish_Time,
    //     CPU_U,
    //     IO_U,
    //     Average_TurnA,
    //     Average_CPUW,
    //     Through_P);

}


// void Output::print(deque<Process*> Process_List, int Finish_Time, int IO_Time, string type)
// {
//     double Total_Turn = 0;
//     double Total_CPUW = 0;
//     double Total_CPU_T = 0;
//     cout << type << "\n";
//     for (int i = 0; i < Process_List.size(); i++)
//     {
//         Total_Turn += Process_List.at(i)->FT - Process_List.at(i)->AT;
//         Total_CPUW += Process_List.at(i)->CW;
//         Total_CPU_T += Process_List.at(i)->TC;

//     double CPU_U = 100 * (double)Total_CPU_T / (double)Finish_Time;
//     double IO_U = 100 * (double)IO_Time / (double)Finish_Time;
//     double Average_TurnA = (double)Total_Turn / (double)Process_List.size();
//     double Average_CPUW = (double)Total_CPUW / (double)Process_List.size();
//     double Through_P = (double)Process_List.size() * 100 / (double)Finish_Time;

//     printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n", Finish_Time,
//         CPU_U,
//         IO_U,
//         Average_TurnA,
//         Average_CPUW,
//         Through_P);

// }
