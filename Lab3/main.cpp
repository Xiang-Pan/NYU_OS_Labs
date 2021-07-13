/*
 * @Author: Xiang Pan
 * @Date: 2021-06-23 18:01:09
 * @LastEditTime: 2021-07-13 00:46:34
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/main.cpp
 * xiangpan@nyu.edu
 */

#include "input_handler.hpp"
#include "process_manager.hpp"
#include "utils.hpp"


using namespace std;

/**
 * @description: main
 * @param {int} argc
 * @param {char*} argv
 * @return {*}
 */
int main(int argc, char* argv[])
{
    
    InputHandler input_handler(argc, argv);
    input_handler.simulate();
    return 0;
}