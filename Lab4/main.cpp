/*
 * @Author: Xiang Pan
 * @Date: 2021-07-13 04:57:16
 * @LastEditTime: 2021-07-13 04:58:55
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab4/main.cpp
 * xiangpan@nyu.edu
 */
#pragma once
#include "utils.hpp"
#include "input_handler.hpp"
#include "output_handler.hpp"


using namespace std;

int main(int argc, char* argv[])
{
    InputHandler input_handler(argc, argv);
    intput_handler.simulate();
}