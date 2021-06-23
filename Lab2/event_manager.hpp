#pragma once
#include <string>
#include <vector>
#include "event.hpp"
using namespace std;


class EventManager
{
    public:
        EventManager();
        EventManager(InputHandler* input_handler ,deque<Process*> input_process_queue);
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

        void Simulation();

};

EventManager::EventManager(InputHandler* p_input_handler ,deque<Process*> input_process_queue)
{
    input_handler = p_input_handler;
    set_event_queue(input_process_queue);
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
    debug(event);
    cur_event = event;
    event->ih = input_handler;
    event->s = s;
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
    if(event_queue.empty())
    {
        return -1;
    }
    else
    {
        return event_queue.front()->timestamp;
    }
}


void EventManager::Simulation()
{
    Event* evt;
    // event_transition Next_Transition;
    Process* cur_running_process = NULL;
    int cur_time = 0;
    int cur_block_time = 0;
    bool call_scheduler = false;
    // debug(e);
    // int CURRENT_BLOCK_TIME = 0;
    while(evt = get_event())
    {
        // evt.input_handler = g_input_handler;
        // cur_running_process = evt->p; // the process works on
//        cur_time
        cur_time = evt->timestamp;
//        evt->cur_time = cur_time;
        evt->get_new_state();


        evt->make_transition();
        Event* next_event = evt->next_event;
        if(next_event != nullptr)
        {
            put_event(evt->next_event);
        }


        call_scheduler = evt->call_scheduler;
        

        delete evt; 
        evt = nullptr;
        


        if(call_scheduler) 
        {
            if (get_next_event_time() == cur_time)
                continue;           //process next event from Event queue
            call_scheduler = false; // reset call scheduler
            if (cur_running_process == nullptr) 
            {
                cur_running_process = s->get_next_process();
                if (cur_running_process == nullptr)
                    continue;
                // create event to make this process runnable for same time.
                Event* e = new Event(cur_running_process, cur_time, TRANS_TO_RUN);
                put_event(e);
                
            }
        }
    }
    
    // int 


    
}

