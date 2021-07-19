/*
 * @Author: Xiang Pan
 * @Date: 2021-06-23 18:01:09
 * @LastEditTime: 2021-07-17 22:34:27
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/main.cpp
 * xiangpan@nyu.edu
 */

#include "input_handler.hpp"
#include "process_manager.hpp"
#include "system_shared.hpp"
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
    // ProcessManager process_manager;
    // p_process_manager = new ProcessManager();
    InputHandler input_handler(argc, argv);
    
    for (vector<frame_index_type>::size_type ix = 0; ix<frame_table_size; ++ix)
        frame_table_free_pool.push_back(ix);
    input_handler.simulate();
    input_handler.summary();
    return 0;
}