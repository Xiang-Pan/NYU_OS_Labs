#include <deque>
#include <vector>
#include "process.hpp"



using namespace std;
class ProcessManager
{
    public:
        ProcessManager();
        deque<Process*> process_queue;
        void create_process_from_input(InputHandler& input_handler);

    private:
};


ProcessManager::ProcessManager()
{

}

// int string2int(string s)
// {
// object from the class stringstream
//     stringstream ss(s);
//     int x = 0;
//     ss >> x;
//     return x;
// }

void ProcessManager::create_process_from_input(InputHandler& input_handler)
{
    int pid = 0;
    vector<string> tokens;
    while(input_handler.read_inputfile())
    {
        tokens = input_handler.tokens;
        Process* p = new Process();
        p->pid = pid;
        p->AT = string2int(tokens[0]);
        p->TC = string2int(tokens[1]);
        p->CB = string2int(tokens[2]);
        p->IO = string2int(tokens[3]);
        p->RC = p->TC;
        process_queue.push_back(p);
        pid += 1;
    }
}
