/*
 * @Author: Xiang Pan
 * @Date: 2021-07-14 02:04:49
 * @LastEditTime: 2021-07-21 00:54:15
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab4/output_handler.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include <iostream>
#include <iomanip>
#include "io_request.hpp"
#include "system_shared.hpp"

using namespace std;

void log_add(IORequest* p_next_io_request)
{
    if(kIsTrace) 
    {
        cout << kTime << ": "<< setw(5) << p_next_io_request->rid_ << " add "<< p_next_io_request->track_ << endl;
    }
}

void log_finish(IORequest* p_cur_io_request)
{
    if(kIsTrace) 
    {
        // cout << kTime << ": "<< setw(5) << p_cur_io_request->rid_ << " add "<< p_cur_io_request->track_ << endl;
        cout << kTime << ": "<< setw(5) << p_cur_io_request->rid_ << " finish "<< (kTime - p_cur_io_request->arrive_time_) << endl;
    }
}

void log_issue(IORequest* p_cur_io_request)
{
    if(kIsTrace)
    {
        cout<< kTime << ": "
            << setw(5) << p_cur_io_request->rid_ << " issue "
            << p_cur_io_request->track_ << " "
            << kCurTrack << endl;
    }
}