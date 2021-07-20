/*
 * @Author: Xiang Pan
 * @Date: 2021-06-28 19:19:14
 * @LastEditTime: 2021-07-19 17:56:52
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/process_manager.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include <vector>
#include "process.hpp"
#include "utils.hpp"
using namespace std;



class ProcessManager
{
    public:
        vector<Process> process_vec_;
        unsigned long int cur_pid = 0;
        void add_process(Process p)
        {
            process_vec_.push_back(p);
        }
        Process& get_process(unsigned long int pid)
        {
            return process_vec_[pid];
        }
        Process& get_cur_process()
        {
            return get_process(cur_pid);
        }
        void print_all_page_able()
        {
            for(Process&p : process_vec_)
            {
                p.print_page_table();
            }
        }
        void print_all_process_stats()
        {
            for(Process&p : process_vec_)
            {
                p.print_stats();
            }
        }
};





