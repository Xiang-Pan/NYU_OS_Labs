#pragma once
#include <vector>
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
        int VMA_count_;
        int process_num_;
        vector<VMA> VMA_vec_;
        void add_VMA(VMA v)
        {
            VMA_vec_.push_back(v);
        }

        

        // starting_virtual_page ending_virtual_page write_protected[0/1] filemapped[0/1];
};