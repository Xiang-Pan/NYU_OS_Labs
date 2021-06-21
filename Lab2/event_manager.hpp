#pragma once
#include <string>
#include <vector>
#include "event.hpp"
using namespace std;


class EventManager
{
    public:
        deque<Event*> event_queue;  // des layer queue
        // void Initiliaze_Event_Queue(deque<Process*> ProcessList);


        Event* get_event();
        void put_event(Event* e);
        void rm_event(Process* p);
        int get_next_event_time();
        
};


Event* EventManager::get_event()
{
    if (event_queue.empty())
    {
        Event* pe = nullptr;
        return pe;
    }   
    Event* event = event_queue.front();
    event_queue.pop_front();
    return event;
}


// void EventManager::search




void EventManager::rm_event(Process* p)
{
    // deque<Process*>::iterator result = find(event_queue.begin(), event_queue.end(), p); 
    // if(result != event_queue.end())
    // {
    //     event_queue.erase(result);
    // }
    // else
    // {
    //     throw invalid_argument("can not find p with pid");
    // }
}