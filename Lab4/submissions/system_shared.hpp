/*
 * @Author: Xiang Pan
 * @Date: 2021-07-20 02:24:20
 * @LastEditTime: 2021-08-12 19:44:43
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab4/system_shared.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include <iostream>
using namespace std;


unsigned long kTime = 0;

long kCurTrack = 0;
int kCurDirection = -2;

//stat info
unsigned long  total_waittime        = 0;
unsigned long  total_turnaround_time = 0;
unsigned long  total_time            = 0;
unsigned long  total_movement          = 0;
unsigned long  max_waittime          = 0;
unsigned long  requests_count = 0;
double avg_turnaround     = 0;
double avg_waittime       = 0;


bool kIsTrace     = false;
bool kDebugQueue  = false;


