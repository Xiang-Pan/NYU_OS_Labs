/*
 * @Author: Xiang Pan
 * @Date: 2021-06-28 19:40:16
 * @LastEditTime: 2021-07-13 00:16:54
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/pager.hpp
 * xiangpan@nyu.edu
 */
#pragma once

using namespace std;



void update_pte()
{
	
}


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