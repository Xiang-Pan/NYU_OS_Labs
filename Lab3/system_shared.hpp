/*
 * @Author: Xiang Pan
 * @Date: 2021-07-12 23:42:26
 * @LastEditTime: 2021-07-13 00:14:46
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/system_shared.hpp
 * xiangpan@nyu.edu
 */

#define MAX_FRAMES 1024
#define MAX_VPAGES 1024

const int page_table_size = 64;

//32 bit max
typedef struct pte_t 
{
	unsigned present: 1;
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


frame_t frame_table[MAX_FRAMES]; 
// pte_t page_table[MAX_VPAGES];