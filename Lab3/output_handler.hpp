/*
 * @Author: Xiang Pan
 * @Date: 2021-06-23 17:42:26
 * @LastEditTime: 2021-07-13 01:10:38
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/output_handler.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include <string>
#include "input_handler.hpp"
using namespace std;


void log_operation(int inst_count,string op, int vpage)
{
    // debug
    cout << inst_count << ": ==> "<< op << " "<< vpage << endl;
}