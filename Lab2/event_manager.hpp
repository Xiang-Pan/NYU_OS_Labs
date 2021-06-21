#pragma once
#include <string>
#include <vector>
#include "event.hpp"
using namespace std;


class EventManager
{
    public:
        EventManager();
        EventManager(deque<Process*> input_process_queue);
        deque<Event*> event_queue;  // des layer queue
        void set_event_queue(deque<Process*> input_process_queue);
        // void Initiliaze_Event_Queue(deque<Process*> ProcessList);


        Event* get_event();
        void put_event(Event* e);
        void rm_event(Process* p);
        int get_next_event_time();

};

EventManager::EventManager(deque<Process*> input_process_queue)
{
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
        Event* e = new Event(input_process_queue[i],input_process_queue[i]->AT,TRANS_TO_READY);
        event_queue.push_back(e);
    }
}

EventManager::EventManager()
{

}


Event* EventManager::get_event()
{
    if(event_queue.empty())
    {
        debug("event_queue.empty");
        Event* pe = nullptr;
        return pe;
    }   
    Event* event = event_queue.front();
    event_queue.pop_front();
    return event;
}


void EventManager::put_event(Event* e)
{
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
