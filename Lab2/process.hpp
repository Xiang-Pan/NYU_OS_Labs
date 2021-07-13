/*
 * @Author: Xiang Pan
 * @Date: 2021-06-15 20:28:07
 * @LastEditTime: 2021-07-13 16:58:24
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab2/process.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include <string>
#include <iostream>
#include <ostream>
#include "utils.hpp"

using namespace std;


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
        int pid = -1;//ID of this process
        int AT; //Arrival time
        int TC; //Total CPU time
        int CB; //CPU burst
        int IO; //I/O burst

        int RC; //Rest of CPU time
        int CW = 0; //Cpu waiting time
        int FT = -1; //finish time
        int IT = 0; //I/O time


        
        int remaining_CB = 0;
        int dynamic_prio = 0;
        int generated_CB = 0;
        int state_ts = -1;                   //cur state begin time
        int time_in_prev_state = 0;     //previous state time length



        int static_prio;

        int next_time = -1;
        Event_transition next_transition;
        Process_STATE state = state_created;
        Process();

    bool operator == (const Process &p) 
    {
        return (this->pid == p.pid);
    }

    friend ostream &operator<<( ostream &output, const Process &p )
    { 
        output << p.pid <<endl;
        return output;            
    }



    private:

};



Process::Process()
{

}


