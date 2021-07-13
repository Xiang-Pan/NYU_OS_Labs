/*
 * @Author: Xiang Pan
 * @Date: 2021-06-28 19:02:31
 * @LastEditTime: 2021-07-13 00:00:40
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/process.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include <vector>
#include <iostream>
#include "system_shared.hpp"
using namespace std;

class VMA
{
    public:
        int starting_virtual_page_;
        int ending_virtual_page_;
        bool write_protected_;
        bool filemapped_;
        VMA(int starting_virtual_page, int ending_virtual_page, bool write_protected, bool filemapped)
        {
            starting_virtual_page_ = starting_virtual_page;
            ending_virtual_page_ = ending_virtual_page;
            write_protected_ = write_protected;
            filemapped_ = filemapped;
        }

};


class Process
{
    public:
        int pid = -1;
        int VMA_count_;
        // int process_num_;
        vector<VMA> VMA_vec_;

        pte_t page_table_[page_table_size];
        
        void add_VMA(VMA v)
        {
            VMA_vec_.push_back(v);
        }

        friend std::ostream &operator<<( std::ostream &output, const Process &p )
        { 
            output << p.pid << endl;
            return output;
        }

        

        // starting_virtual_page ending_virtual_page write_protected[0/1] filemapped[0/1];
};