/*
 * @Author: Xiang Pan
 * @Date: 2021-07-19 20:55:06
 * @LastEditTime: 2021-07-19 20:57:59
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/test.cpp
 * xiangpan@nyu.edu
 */
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    unsigned int a = 0x200000;
    a = a | 0x80000000;
    cout << hex << a << endl;
    // unsigned int b = 
}