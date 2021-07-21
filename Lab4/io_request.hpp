/*
 * @Author: Xiang Pan
 * @Date: 2021-07-20 03:37:44
 * @LastEditTime: 2021-07-20 21:48:54
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab4/io_request.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include <iostream>
#include <deque>

using namespace std;


class IORequest
{
    public:
        int rid_            = 0;
        int arrive_time_    = 0;
        int track_          = 0;
        int start_time_     = -1;
        int end_time_       = -1;

        IORequest(int rid, int arrive_time, int track)
        {
            rid_ = rid;
            arrive_time_ = arrive_time;
            track_ = track;
        }
};

//deque<IORequest> io_request_queue;
