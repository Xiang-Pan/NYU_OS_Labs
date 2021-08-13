/*
 * @Author: Xiang Pan
 * @Date: 2021-07-20 03:37:44
 * @LastEditTime: 2021-08-12 19:38:13
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
        long rid_            = 0;
        long arrive_time_    = 0;
        long track_          = 0;
        long start_time_     = -1;
        long end_time_       = -1;

        IORequest(long rid, long arrive_time, long track)
        {
            rid_ = rid;
            arrive_time_ = arrive_time;
            track_ = track;
        }
};

//deque<IORequest> io_request_queue;
