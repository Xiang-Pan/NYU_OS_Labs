#pragma once
#include <vector>
#include "process.hpp"
#include "utils.hpp"
using namespace std;



class ProcessManager
{
    public:
        vector<Process> process_vec; 
        void add_process(Process p)
        {
            process_vec.push_back(p);
        }
};


