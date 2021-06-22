#pragma once
#include <string>


enum Process_STATE
{
    state_created,
    state_ready,
    state_running,
    state_blocked,
    state_preempt
};

class Process
{
    public:
        int pid;//ID of this process
        int AT; //Arrival time
        int TC; //Total CPU time
        int CB; //CPU burst
        int IO; //I/O burst

        int RC; //Rest of CPU time
        int CW; //Cpu waiting time
        int FT; //finish time
        int IT; //I/O time
        // int time_in_prev_state; //
        int remaining_CB = 0;
        int dynamic_prio = 0;
        // int generated_CB;
        int state_ts;               //cur state begin time
        int time_in_prev_state;     //previous state time length
        
        // int nexttime;

        // int created;
        Process_STATE state = state_created;
        Process();

    bool operator == (const Process &p) 
    {
        return (this->pid == p.pid);
    }
    private:

    // int static_Prio;
    // int dynamic_Prio;
    // int timeInPrevState;


};



Process::Process()
{

}

    // IT = 0;
    // CW = 0;
    // state = STATE_CREATED;
    // timeInPrevState = 0;
    // remaining_CB = 0;
    // static_Prio = 0;
    // dynamic_Prio = 0;
    // created = 0;
    

