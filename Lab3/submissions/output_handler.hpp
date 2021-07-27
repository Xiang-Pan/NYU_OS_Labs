/*
 * @Author: Xiang Pan
 * @Date: 2021-06-23 17:42:26
 * @LastEditTime: 2021-07-27 18:54:28
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/output_handler.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include <string>
#include "input_handler.hpp"
#include "system_shared.hpp"
#include "process.hpp"


class Process;

using namespace std;


void log_operation(int inst_count, char op, int vpage)
{
    // debug
    if(output_O)
    {
        cout << inst_count << ": ==> "<< op << " "<< vpage << endl;
    }
}

void log_segprot()
{
    if(output_O) 
    {
        cout << " SEGPROT" << endl;
    }
}

void log_IN()
{
    if(output_O) 
    {
        cout << " IN" << endl;
    }
}

void log_FIN()
{
    if(output_O) 
    {
        cout << " FIN" << endl;
    }
} 

void log_ZERO()
{
    if(output_O) 
    {
        cout << " ZERO" << endl;
    }
}

void log_MAP(int physical_frame)
{
    if(output_O) 
    {
        cout << " MAP " << physical_frame << endl;
    }

}


void log_SEGV()
{
    if(output_O) 
    {
        cout << " SEGV" << endl;
    }
}

void log_UMAP(unsigned int umap_pid, unsigned int umap_vpage)
{
    if(output_O) 
    {
        cout << " UNMAP " << umap_pid << ":" << umap_vpage << endl;
    }
}

void log_FOUT()
{
    if(output_O) 
    {
        cout << " FOUT"<< endl;
    }
}

void log_OUT()
{
    if(output_O) 
    {
        cout << " OUT"<< endl;
    }
}

void log_ASELECT(frame_index_type victim_index)
{
    if(output_aging_info)
    {
        cout << "ASELECT " << victim_index << endl;
    }
}


void log_operation_result(pte_t& pte, vector<Process>& process_vec)
{
    if(output_all_pagetable_after_instruction)
    {
        if(pte.in_VMA)
        {
            
        }

    }
    if(output_current_pagetable_after_instruction)
    {

    }
    if(output_framtable_after_instruction)
    {
        
    }
}

void print_frame_table()
{
    printf("FT: ");
	for (size_t i = 0; i < frame_table_size; i++)
	{
        frame_t& frame = frame_table[i];
		if(frame.mapped == true)
		{
			printf("%d:%d ", frame.pid, frame.vmp);
		}
		else
		{
			printf("* ");
		}
	}
	cout << endl;
}