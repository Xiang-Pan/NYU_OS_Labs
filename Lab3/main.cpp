#include "intput_handler.hpp"
#include "utils.hpp"


using namespace std;

int main(int argc, char* argv[])
{
    InputHandler input_handler(argc, argv);
    input_handler.simulate();
    
    return 0;
}