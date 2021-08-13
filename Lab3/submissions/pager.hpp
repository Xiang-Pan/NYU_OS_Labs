/*
 * @Author: Xiang Pan
 * @Date: 2021-06-28 19:40:16
 * @LastEditTime: 2021-07-27 19:08:43
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/pager.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include <iostream>
#include <iomanip>
#include "system_shared.hpp"
#include "output_handler.hpp"

using namespace std;


class Pager
{
    public:
        int frame_count_;
        ProcessManager* p_process_manager_;
        int victim_index_ = 0;
        frame_index_type allocate_frame(vpage_index_type vpage, Process& cur_process);
        bool page_fault_handler(pte_t& pte, unsigned int vpage, Process& cur_process);
        
        virtual frame_index_type select_victim_frame();
//        Pager();
        Pager(ProcessManager* p_process_manager)
        {
             p_process_manager_ = p_process_manager;
        }

    Pager();
    // frame_t* select_victim_frame() = 0;
};

Pager::Pager()
{

}


frame_index_type Pager::select_victim_frame()
{
    return victim_index_;
}


frame_index_type Pager::allocate_frame(vpage_index_type vpage, Process& cur_process) 
{
    //allocate_frame_from_free_list();
    frame_index_type index = -1; 
    if(frame_table_free_pool.empty() != true)
    {
        index = frame_table_free_pool.front();
        frame_table_free_pool.pop_front();
    }
    else  //select_victim_frame
    {
        index = select_victim_frame();
//
        // unmap
        unsigned int umap_pid  = frame_table[index].pid;
        unsigned int umap_vpage = frame_table[index].vmp;
        
        Process& umap_p = p_process_manager_->get_process(umap_pid);
        umap_p.umap(umap_vpage);
    }
    // set frame info
    frame_table[index].pid = cur_process.pid_;
    frame_table[index].vmp = vpage;
    frame_table[index].mapped = true;
    return index;
}


bool Pager::page_fault_handler(pte_t& pte, unsigned int vpage, Process& cur_process)
{
    // first determine that the vpage can be accessed
    pte.in_VMA = false;
    for (VMA& vma : cur_process.VMAs_)
    {
        if (vpage >= vma.starting_virtual_page_ && vpage <= vma.ending_virtual_page_)
        {
            pte.in_VMA = true;                                  //in_VMA
            pte.file_mapped = vma.file_mapped_;
            pte.write_protected = vma.write_protected_;
            break;
        }
    }

    if(pte.in_VMA)
    {
        
        pte.physical_frame = allocate_frame(vpage, cur_process);
        pte.present = true;
        // debug(pte.paged_out);
        if(pte.paged_out) 
        {
            log_IN();
            cur_process.ins_+= 1;
            total_cost += kIns;
        }
        else if (pte.file_mapped) 
        {
            log_FIN();
            cur_process.fins_ += 1;
            total_cost += kFins;
        }
        else
        {
            log_ZERO();
            cur_process.zeros_ += 1;
            total_cost += kZeros;
        }
        // debug(pte.physical_frame);
        log_MAP(pte.physical_frame);
        cur_process.maps_ += 1;
        total_cost += kMaps;
        frame_table[pte.physical_frame].age = 0x00000000;
        return true;
    }
    else
    {
        log_SEGV();
        cur_process.segv_ += 1;
        total_cost += kSegv;
        return false;
    }
    return false;
    

}


class FIFOPager: public Pager
{
    public:
        virtual frame_index_type select_victim_frame();
        FIFOPager(ProcessManager* p_process_manager)
        {
            p_process_manager_ = p_process_manager;
        }
};

frame_index_type FIFOPager::select_victim_frame()
{
    victim_index_ = victim_index_ % frame_table_size;
    log_ASELECT(victim_index_);
    return victim_index_++;
}


class AgingPager: public Pager
{
    public:
        AgingPager(ProcessManager* p_process_manager)
        {
            p_process_manager_ = p_process_manager;
        }
        virtual frame_index_type select_victim_frame();
};

frame_index_type AgingPager::select_victim_frame()
{
    // victimindex = (victimindex == (Framenum - 1)) ? 0 : victimindex + 1;
    victim_index_ = victim_index_ % frame_table_size;

    int start = victim_index_;  
    if(output_aging_info)
    {
        cout << "ASELECT " << start<< "-" << (start-1) % frame_table_size << " | ";
        int i = start;
        cout << i << ":"<< hex << frame_table[i].age << dec << " ";
        i = (i + 1) % frame_table_size;
        while(i != (start) % frame_table_size)
        {
            cout << i << ":"<< hex << frame_table[i].age << dec << " ";
            i = (i+1) % frame_table_size;
        }
        cout << "| " << victim_index_ << endl;
    }

    
    for(frame_t& f : frame_table)
    {
        f.age = f.age >> 1;
        if(p_process_manager_->get_process(f.pid).page_table_[f.vmp].referenced == true)
        {
            f.age |= 0x80000000;
            p_process_manager_->get_process(f.pid).page_table_[f.vmp].referenced = false;
        }
    }

    //get min
    int i = (start + 1)% frame_table_size;
    while(i != start)
    {
        if(frame_table[i].age < frame_table[victim_index_].age)
		{
			victim_index_ = i;
		}
        i = (i+1) % frame_table_size;
    }

    if(output_aging_info)
    {
        cout << "ASELECT " << start<< "-" << (start-1) % frame_table_size << " | ";
        int i = start;
        cout << i << ":"<< hex << frame_table[i].age << dec << " ";
        i = (i + 1) % frame_table_size;
        while(i != (start) % frame_table_size)
        {
            cout << i << ":"<< hex << frame_table[i].age << dec << " ";
            i = (i+1) % frame_table_size;
        }
        cout << "| " << victim_index_ << endl;
    }
    frame_table[victim_index_].age = 0x00000000;
	return victim_index_++;
}


class RandomPager : public Pager
{
    public:
        int (*p_random_function_)();
        RandomPager(ProcessManager* p_process_manager)
        {
            p_process_manager_ = p_process_manager;
        }
        virtual frame_index_type select_victim_frame();
};

frame_index_type RandomPager::select_victim_frame()
{
    return get_random_num();
}


class ClockPager : public Pager 
{
public:
    ClockPager(ProcessManager* p_process_manager) : Pager(p_process_manager) {}
    virtual frame_index_type select_victim_frame();
};

frame_index_type ClockPager::select_victim_frame()
{
    victim_index_ = victim_index_ % frame_table_size;
    unsigned int start = victim_index_;
    unsigned int counter = 0;
    while (true) 
    {
        counter++;
        frame_t& frame = frame_table[victim_index_];
        pte_t  & pte = p_process_manager_->get_process(frame.pid).page_table_[frame.vmp];
        if(pte.referenced)
        {
            pte.referenced = false;
            victim_index_ = (victim_index_+1) % frame_table_size;
        }
        else
        {
            break;
        }
    }
    return victim_index_++;
}

class NRUPager: public Pager
{
    public:
        NRUPager(ProcessManager* p_process_manager): Pager(p_process_manager){}
        virtual frame_index_type select_victim_frame();
        static const unsigned int kResetCycle_ = 50;
        unsigned long cur_inst_count_ = 0;
        unsigned long last_reset_inst_count_ = -1;
};

frame_index_type NRUPager::select_victim_frame()
{
    // 0:- Not Referenced, Not Modified
    // 1:- Not Referenced, Modified
    // 2:- Referenced, Not Modified
    // 3:- Referenced, Modified
    victim_index_ = victim_index_ % frame_table_size;
    cur_inst_count_ = kInstCount;
    // debug(last_reset_inst_count_);
    // debug(cur_inst_count_);
    vector<unsigned int> classes(4, -1);
    frame_index_type start = victim_index_;
    unsigned int counter  = 0;
    bool reset = cur_inst_count_-last_reset_inst_count_ >= kResetCycle_;
    while(counter<frame_table_size)
    {
        counter++;
        frame_t& frame = frame_table[victim_index_];
        pte_t  & pte = p_process_manager_->get_process(frame.pid).page_table_[frame.vmp];

        int level = pte.referenced * 2 + pte.modified;
        if(classes[level] == -1)  // save the first entry
        {
            classes[level] = victim_index_;
        }
        if(level == 0 && !reset) // find level 0 and no need to reset
        {
            break;
        }
        if(reset) 
        {
            pte.referenced = false; 
        }
        victim_index_ = ((victim_index_+1) % frame_table_size);
    }
    if(reset) 
    {
        last_reset_inst_count_ = cur_inst_count_; 
    }
    int level = 0;
    for(frame_index_type i : classes)
    {
        if(i != -1)
        {
            victim_index_ = i;
            break;
        }
        level += 1;
    }
    if(output_aging_info)
    {
        cout << "ASELECT: hand="    << setw(2) << start
                                    << " " << setw(1)   << reset
                                    << " | " << setw(1) << level
                                    << " " << setw(2)   << victim_index_
                                    << " " << setw(2)   << counter << setw(0) << endl;
    }
    return (victim_index_)++;

}


class WorkingSetPager: public Pager
{
    public:

        virtual frame_index_type select_victim_frame();
        vector<unsigned long> last_used;
        const int TAU = 49;
        WorkingSetPager(ProcessManager* p_process_manager) : Pager(p_process_manager), last_used(frame_table_size){}
};

frame_index_type WorkingSetPager::select_victim_frame()
{
    victim_index_ = victim_index_ % frame_table_size;
    unsigned int start = victim_index_;
    unsigned int oldest = victim_index_;
    frame_index_type counter = 0;
    while(counter < frame_table_size)
    {
        frame_t& frame = frame_table[victim_index_];
        pte_t  & pte = p_process_manager_->get_process(frame.pid).page_table_[frame.vmp];
        if (pte.referenced) 
        {
            last_used[victim_index_] = kInstCount;
            pte.referenced = false;
        }
        else if (kInstCount-last_used[victim_index_] > TAU) 
        {
            oldest = victim_index_;
            break;
        }
        oldest = last_used[victim_index_]<last_used[oldest] ? victim_index_ : oldest;
        victim_index_ = (victim_index_+1) % frame_table_size;
        counter += 1;
    }
    victim_index_ = oldest;
    return victim_index_++;
}



