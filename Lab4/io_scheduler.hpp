/*
 * @Author: Xiang Pan
 * @Date: 2021-07-20 02:25:00
 * @LastEditTime: 2021-07-20 03:58:58
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab4/io_scheduler.hpp
 * xiangpan@nyu.edu
 */

class IOScheduler
{
    public: 
        IOScheduler();
        virtual void add_io_request(IORequest& p);
};


void IOScheduler::add_io_request(IORequest& p)
{

}
        

IOScheduler::IOScheduler()
{

}

class FIFOIOScheduler: public IOScheduler
{
    
};