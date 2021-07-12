#pragma once

using namespace std;

//32 bit max
typedef struct pte_t 
{
	unsigned PRESENT : 1;
	unsigned WRITE_PROTECT : 1;
	unsigned MODIFIED : 1;
	unsigned REFERENCED : 1;
	unsigned PAGEDOUT : 1;
	unsigned FILEMAPPED : 1;
	unsigned PHYSICAL_FRAME : 7;
} pte_t;

typedef struct frame_t
{
	int frame_index;
	int vmp;
	int process;
	unsigned mapped : 1;
	unsigned long long currenttime;
	unsigned age : 32;
} frame_t;


class Pager
{
    int frame_count;
    virtual frame_t* select_victim_frame() = 0;
};

class FIFOPager: public Pager
{
    public:
        int victim_index = -1;
        frame_t* select_victim_frame();
        // frame_t** frame_table;
        // 

};

// frame_t* FIFOPager::select_victim_frame()
// {
// 	victim_index = (victim_index == (frame_count-1)) ? 0 : victim_index + 1;
// 	return frame_table[victim_index];
// }