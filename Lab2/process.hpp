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
        int CW = 0; //Cpu waiting time
        int FT = -1; //finish time
        int IT = 0; //I/O time
        // int time_in_prev_state; //
        int remaining_CB = 0;
        int dynamic_prio = 0;
        int generated_CB = 0;
        int state_ts = -1;                   //cur state begin time
        int time_in_prev_state = 0;     //previous state time length



        int static_prio;
        // int dynamic_prio;


        
        
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
    

