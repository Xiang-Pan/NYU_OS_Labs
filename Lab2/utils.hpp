/*
 * @Author: your name
 * @Date: 2021-06-16 11:55:39
 * @LastEditTime: 2021-07-13 17:12:18
 * @LastEditors: Xiang Pan
 * @Description: In User Settings Edit
 * @FilePath: /Lab2/submissions/utils.hpp
 */
#pragma once


extern bool verbose = false;


#define VNAME(name) (#name)
#define debugging_enabled false 
#define debug(x) do { \
  if (debugging_enabled) { std::cerr << VNAME(x)<<":"<< x << std::endl; }\
} while (0)

#define debug_vector(v) do { \
    if (debugging_enabled) {\
        for(int i=0; i<v.size(); i++)\
        {\
            std::cerr << i <<":"<< v[i] << std::endl;\
        }\
    } \
} while (0)

// #include <string>
enum Transition_type
{
    created_ready,
    ready_running,
    running_blocked,
    blocked_ready,
    running_preempt, // running_ready,
    preempt_running
};

enum Event_transition
{
    TRANS_TO_READY,
    TRANS_TO_RUN,
    TRANS_TO_BLOCK,
    TRANS_TO_PREEMPT,
    // TRANS_TO_DONE
};