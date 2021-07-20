/*
 * @Author: Xiang Pan
 * @Date: 2021-07-12 23:42:26
 * @LastEditTime: 2021-07-20 00:31:04
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/system_shared.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include <sstream>
#include <iostream>
using namespace std;

// #define MAX_FRAMES 1024
// #define MAX_VPAGES 1024
typedef unsigned int frame_index_type;
typedef unsigned int vpage_index_type;

const int kPageTableSize = 64;
const int kMaxFrameTableSize = 64;
frame_index_type frame_table_size = 0;
int kInstCount = 0;


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

void reset_pte(pte_t& pte)
{
	pte.present = false;
	pte.write_protected = false;
	pte.modified = false;
	pte.referenced = false;
	pte.in_VMA = false;
	pte.paged_out = false;
	pte.file_mapped = false;
}

typedef struct frame_t
{
	frame_index_type frame_index;
	vpage_index_type vmp;
	int pid;
	unsigned mapped : 1;
	// unsigned long long currenttime;
	unsigned int age : 32;
	// unsigned int age;
	frame_t(): age(0x00000000)
	{
	}
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
bool output_aging_info = false;                          // aging


ifstream randomfile_stream;
int cur_random_seed = 0;
int total_random_count = 0;

void read_randomfile()
{
    string line;
    getline(randomfile_stream, line);
    stringstream ss(line);
    ss >> total_random_count;
}


int get_random_seed()
{
    string line;
    if(!getline(randomfile_stream, line))
    {
        randomfile_stream.clear();                 // clear fail and eof bits
        randomfile_stream.seekg(0, std::ios::beg); // back to the start!
		read_randomfile();
        getline(randomfile_stream, line);
    }
    stringstream ss(line);
    ss >> cur_random_seed;
    return cur_random_seed;
}

int get_random_num()
{
    get_random_seed();
	// debug(cur_random_seed);
    int burst = frame_table_size;
    return (cur_random_seed % burst);;
}

pte_t& get_pte_by_frame(frame_t& f)
{ 
	
}
// ProcessManager process_manager;
// extern ProcessManager* process_manager;


// read/write (load/store) instructions count as 1, context_switches instructions=130, process exits instructions=1250. In addition if the following operations counts as follows:

// maps=300, unmaps=400, ins=3100, outs=2700, fins=2800, fouts=2400, zeros=140, segv=340, segprot=420

// pte_t page_table[MAX_VPAGES];