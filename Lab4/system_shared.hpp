/*
 * @Author: Xiang Pan
 * @Date: 2021-07-20 02:24:20
 * @LastEditTime: 2021-07-21 02:46:02
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab4/system_shared.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include <iostream>
using namespace std;


int kTime = 0;


int kCurTrack = 0;
int kCurDirection = -2;

//stat info
int total_waittime        = 0;
int total_turnaround_time = 0;
int total_time            = 0;
int total_movement          = 0;
int max_waittime          = 0;
int requests_count = 0;
double avg_turnaround     = 0;
double avg_waittime       = 0;



bool kIsTrace     = false;
bool kDebugQueue  = false;


