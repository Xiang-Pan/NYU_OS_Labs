/*
 * @Author: Xiang Pan
 * @Date: 2021-06-23 18:01:41
 * @LastEditTime: 2021-07-20 19:00:38
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab4/utils.hpp
 * xiangpan@nyu.edu
 */
#pragma once
#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
using namespace std;


#define VNAME(name) (#name)
#define debugging_enabled true 
#define debug(x) do { \
  if (debugging_enabled) { std::cerr << VNAME(x)<<":"<< x << std::endl; }\
} while (0)

#define debug_vector(v) do { \
    if (debugging_enabled) {\
        for(long unsigned int i=0; i<v.size(); i++)\
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

float string2float(string s)
{
    std::string::size_type sz;     // alias of size_t
    float f = std::stof (s,&sz);
    return f;
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
}

template<typename T>
T& createNullRef() { return *static_cast<T*>(nullptr); }
