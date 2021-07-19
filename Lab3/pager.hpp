/*
 * @Author: Xiang Pan
 * @Date: 2021-06-28 19:40:16
 * @LastEditTime: 2021-07-19 01:21:40
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/pager.hpp
 * xiangpan@nyu.edu
 */
#pragma once
// #include "input_handler.hpp"
#include "system_shared.hpp"
#include "output_handler.hpp"



using namespace std;



// void update_pte()
// {
	
// }


class Pager
{
    public:
        int frame_count_;
        ProcessManager* p_process_manager_;
        int victim_index_ = 0;
        frame_index_type allocate_frame(vpage_index_type vpage, Process& cur_process);
        void page_fault_handler(pte_t& pte, unsigned int vpage, Process& cur_process);
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


// frame_t *get_frame() 
// {
//     frame_t *frame = allocate_frame_from_free_list(); 
//     if (frame == NULL) 
//         frame = THE_PAGER->select_victim_frame(); return frame; 
// }

// frame_t* get_frame_index



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


void Pager::page_fault_handler(pte_t& pte, unsigned int vpage, Process& cur_process)
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
        pte.present = true;
        pte.physical_frame = allocate_frame(vpage, cur_process);
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
        // pager->age_operation(pte.frame);
    }
    else
    {
        log_SEGV();
        cur_process.segv_ += 1;
        total_cost += kSegv;
    }
    

}


class FIFOPager: public Pager
{
    public:
        // int victim_index = 0;

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
        // int hand = 0;
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
    // reset referenced  
    for(frame_t& f : frame_table)
    {
        f.age = f.age >> 1;
        if(p_process_manager_->get_process(f.pid).page_table_[f.vmp].referenced == true)
        {
            f.age = (f.age | 0x80000000);
            p_process_manager_->get_process(f.pid).page_table_[f.vmp].referenced = false;
        }
    }
	int min = victim_index_;
	for(int i = 0; i < frame_table_size; i++)
	{
		if(frame_table[min].age == 0)
		{
			victim_index_ = min;
			return victim_index_;
		}
		if(frame_table[min].age < frame_table[victim_index_].age)
		{
			victim_index_ = min;
		}
        min = min % frame_table_size;
		// min  = (min == (frame_table_size - 1)) ? 0 : min + 1;
	}
	frame_table[victim_index_].age = 0x00000000;
	return victim_index_++;
}


// frame_t* FIFOPager::select_victim_frame()
// {
// 	victim_index = (victim_index == (frame_count-1)) ? 0 : victim_index + 1;
// 	return frame_table[victim_index];
// }