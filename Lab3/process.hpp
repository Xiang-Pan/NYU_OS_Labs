/*
 * @Author: Xiang Pan
 * @Date: 2021-06-28 19:02:31
 * @LastEditTime: 2021-07-19 01:30:26
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/process.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include <vector>
#include <iostream>
#include "system_shared.hpp"
#include "output_handler.hpp"
using namespace std;

class VMA
{
    public:
        unsigned int starting_virtual_page_;
        unsigned int ending_virtual_page_;
        bool write_protected_;
        bool file_mapped_;
        VMA(int starting_virtual_page, int ending_virtual_page, bool write_protected, bool filemapped)
        {
            starting_virtual_page_ = starting_virtual_page;
            ending_virtual_page_ = ending_virtual_page;
            write_protected_ = write_protected;
            file_mapped_ = filemapped;
        }

};




class Process
{
    public:
        int pid_ = -1;

        int VMA_count_;
        vector<VMA> VMAs_;

        pte_t page_table_[kPageTableSize];
        

        //status stat
        unsigned long int segprot_ = 0;
        unsigned long int segv_ = 0;
        unsigned long int ins_ = 0;
        unsigned long int fins_ = 0;
        unsigned long int zeros_ = 0;
        unsigned long int maps_ = 0;
        unsigned long int unmaps_ = 0;
        unsigned long int outs_ = 0;
        unsigned long int fouts_ = 0;
        
        void add_VMA(VMA v)
        {
            VMAs_.push_back(v);
        }

        friend std::ostream &operator<<( std::ostream &output, const Process &p )
        { 
            output << p.pid_ << endl;
            return output;
        }

        void print_page_table()
        {
            cout<<"PT["<<to_string(pid_)<<"]: ";
           	for (int i = 0; i < kPageTableSize; i++)
            {
                pte_t& pte = page_table_[i];
                if(pte.present == 0 && pte.paged_out == 1)
                {
                    printf("# ");
                }
                else if(pte.present == 0 && pte.paged_out == 0)
                {
                    printf("* ");
                }
                else
                {
                    string content = to_string((long long int)(i)) + ':';
                    content = (pte.referenced == 1) ? (content + 'R') : (content + '-');
                    content = (pte.modified == 1) ? (content + 'M') : (content + '-');
                    content = (pte.paged_out == 1) ? (content + 'S') : (content + '-');
                    content = content + ' ';
                    cout << content;
                }
            }
            cout << endl;
        }
        void print_stats()
        {
            printf("PROC[%d]: U=%lu M=%lu I=%lu O=%lu FI=%lu FO=%lu Z=%lu SV=%lu SP=%lu\n",
			pid_,
			unmaps_, 
            maps_, 
            ins_, 
            outs_,
			fins_, 
            fouts_, 
            zeros_,
			segv_, 
            segprot_);
        }

        pte_t& get_pte(int vpage)
        {
            return page_table_[vpage];
        }


        void umap(unsigned int umap_vpage)
        {
            log_UMAP(pid_, umap_vpage);
            unmaps_ += 1;
            total_cost += kUmaps;
            pte_t& umap_pte = get_pte(umap_vpage);
            umap_pte.present = false;
            umap_pte.referenced = false;

            if(umap_pte.modified) 
            {
                umap_pte.paged_out = !umap_pte.file_mapped;
                if(umap_pte.file_mapped)
                {
                    log_FOUT();
                    fouts_ += 1;
                    total_cost += kFouts;
                }
                else
                {
                    log_OUT();
                    outs_ += 1;
                    total_cost += kOuts;
                }
                // umap_pte.paged_out = !umap_pte.file_mapped;
                // if(output_O) 
                // {
                //     cout << (umap_pte.file_mapped ? " FOUT" : " OUT") << endl;
                // }
            }
        }



        // void umap()
        // {
        //     Process& process_rv = process_pool[pid_rv];
        //     process_rv.unmaps++;
        //     total
        //     COST += COST_TABLE::UNMAPS;
        // }


        void process_output()
        {
            // printf("PROC[%d]: U=%lu M=%lu I=%lu O=%lu FI=%lu FO=%luZ=%lu SV=%lu SP=%lu\n",pid_,pstats->unmaps, pstats->maps, pstats->ins, pstats->outs,pstats->fins,pstats->fouts, pstats->zeros,pstats->segv, pstats->segprot);
        }

        // starting_virtual_page ending_virtual_page write_protected[0/1] filemapped[0/1];
};




// VMA output
std::ostream &operator<<( std::ostream &output, const pte_t &pte )
{ 
    if (pte.present) 
    {
        output 
        << (pte.referenced ? "R" : "-")
        << (pte.modified ? "M" : "-")
        << (pte.paged_out ? "S " : "- ");
    }
    else 
    {
        output << (pte.paged_out ? "# " : "* ");
    }
    return output;
}

ostream& operator << (ostream& output, const vector<pte_t>& page_table) 
{
    for (size_t i = 0; i < page_table.size(); i++) 
    {
        const pte_t& pte = page_table[i];
        if (pte.present) 
        {
            output << i << ":" << pte;
        }
        else 
        {
            output << (pte.paged_out ? "# " : "* ");
        }
    }
    return output;
}


// ostream& operator << (ostream& os, const vector<Frame>& frame_table) {
//     for (size_t i = 0; i < frame_table.size(); i++) {
//         const Frame& frame = frame_table[i];
//         if (frame.mapped) {
//             os << frame.pid_rv << ":" << frame.vpage_rv << " ";
//         } else {
//             os << "* ";
//         }
//     }
//     return os;
// }