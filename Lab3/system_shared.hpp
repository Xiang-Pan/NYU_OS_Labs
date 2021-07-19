/*
 * @Author: Xiang Pan
 * @Date: 2021-07-12 23:42:26
 * @LastEditTime: 2021-07-19 01:21:13
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/system_shared.hpp
 * xiangpan@nyu.edu
 */
#pragma once

// #include "process_manager.hpp"


// #define MAX_FRAMES 1024
// #define MAX_VPAGES 1024
typedef unsigned int frame_index_type;
typedef unsigned int vpage_index_type;

const int kPageTableSize = 64;
const int kMaxFrameTableSize = 64;
frame_index_type frame_table_size = 0;


//32 bit max
typedef struct pte_t 
{
	unsigned int present: 		  1;  //valid
	unsigned int write_protected: 1;
	unsigned int modified:		  1;
	unsigned int referenced:	  1;
	unsigned int in_VMA:		  1;
	unsigned int paged_out: 	  1;
	unsigned int file_mapped:	  1;
	unsigned int physical_frame : 7;
	pte_t(): paged_out(0), in_VMA(0), write_protected(0),file_mapped(0),present(0),referenced(0),modified(0)
	{
	}
} pte_t;

typedef struct frame_t
{
	frame_index_type frame_index;
	vpage_index_type vmp;
	int pid;
	unsigned mapped : 1;
	unsigned long long currenttime;
	unsigned age : 32;
} frame_t;

frame_t frame_table[kMaxFrameTableSize];
deque<frame_index_type> frame_table_free_pool;

// frame_t frame_table[MAX_FRAMES]; 


unsigned long long total_cost;

enum CostTable 
{
	kReadWrite 		 = 1,
	kContextSwitches = 130,
	kProcessExits	 = 1250,
    kMaps            = 300,
    kUmaps           = 400,
    kIns             = 3100,
    kOuts            = 2700,
    kFins            = 2800,
	kFouts           = 2400,
	kZeros           = 140,
	kSegv            = 340,
    kSegprot         = 420
};



bool output_O = false;
bool output_pagetable = false;
bool output_frametable = false;
bool output_summary = false;


bool output_current_pagetable_after_instruction = false; // x 
bool output_all_pagetable_after_instruction = false;     // y
bool output_framtable_after_instruction = false;         // f
bool output_aging_info = false;                         // aging


pte_t& get_pte_by_frame(frame_t& f)
{ 
	
}
// ProcessManager process_manager;
// extern ProcessManager* process_manager;


// read/write (load/store) instructions count as 1, context_switches instructions=130, process exits instructions=1250. In addition if the following operations counts as follows:

// maps=300, unmaps=400, ins=3100, outs=2700, fins=2800, fouts=2400, zeros=140, segv=340, segprot=420

// pte_t page_table[MAX_VPAGES];