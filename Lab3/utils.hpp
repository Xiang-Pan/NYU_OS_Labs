#pragma once
#include <string>

using namespace std;


#define VNAME(name) (#name)
#define debugging_enabled true 
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


int string2int(string s)
{
    stringstream ss(s);
    int x = 0;
    ss >> x;
    return x;
}

bool string2bool(string s)
{
    if(s != "0" && s != "1")
    {
        debug("srting2bool input error");
        debug(s);
    }
    if(s == "0")
    {
        return false;
    }
    if(s == "1")
    {
        return true;
    }
    return false;
    // except()
    

}