/*
 * @Author: Xiang Pan
 * @Date: 2021-06-28 19:02:31
 * @LastEditTime: 2021-07-27 19:04:17
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

        void umap_all()
        {
            for(size_t i = 0; i < kPageTableSize; i++)
            {
                pte_t& pte = page_table_[i];
                if(pte.present)
                {
                    umap(i, true);
                }
                else
                {
                    reset_pte(pte);
                }

            }
        }


        void umap(unsigned int umap_vpage, bool is_exit = false)
        {
            log_UMAP(pid_, umap_vpage);
            unmaps_ += 1;
            total_cost += kUmaps;
            pte_t& umap_pte = get_pte(umap_vpage);

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
                    if(is_exit)
                    {
                        
                    }
                    else
                    {
                        log_OUT();
                        outs_ += 1;
                        total_cost += kOuts;
                    }

                }
            }
            if(is_exit)
            {
                reset_pte(umap_pte);
//                cout<<"reset"<<umap_pte.paged_out<<page_table_[umap_vpage].paged_out;
                // umap_pte.paged_out = false;
                frame_table[umap_pte.physical_frame].mapped = false;
                frame_table_free_pool.push_back(umap_pte.physical_frame);
            }
            else
            {
                bool paged_out = umap_pte.paged_out;
                reset_pte(umap_pte);
                umap_pte.paged_out = paged_out;
            }
           
            // umap_pte.modified = false;
            // umap_pte.present = false;
            // umap_pte.referenced = false;
        }

        void process_output()
        {
            // printf("PROC[%d]: U=%lu M=%lu I=%lu O=%lu FI=%lu FO=%luZ=%lu SV=%lu SP=%lu\n",pid_,pstats->unmaps, pstats->maps, pstats->ins, pstats->outs,pstats->fins,pstats->fouts, pstats->zeros,pstats->segv, pstats->segprot);
        }

};

