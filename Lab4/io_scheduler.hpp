/*
 * @Author: Xiang Pan
 * @Date: 2021-07-20 02:25:00
 * @LastEditTime: 2021-08-12 19:27:55
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab4/io_scheduler.hpp
 * xiangpan@nyu.edu
 */
#include <queue>
#include <deque>
#include <limits>
#include <cmath>
#include "system_shared.hpp"

using namespace std;

class IOScheduler
{
    public: 
        IOScheduler();
        
        queue<IORequest*> all_requests_queue_;
        vector<IORequest*> arrived_requests_vec_;
        deque<IORequest*> active_queue_;
        deque<IORequest*> add_queue_;
        // int all_requests_vec_index_ = 0;

        // queue<IORequest*> all_added_requests_queue_;

        virtual void add_io_request(IORequest* p);
        virtual IORequest* get_next_io_request();
        virtual bool empty()
        {                
            return active_queue_.empty();
        }
};


void IOScheduler::add_io_request(IORequest* p)
{
    active_queue_.emplace_back(p); 
}

IORequest* IOScheduler::get_next_io_request()
{
    debug("default");
    IORequest* p_r = active_queue_.front();
    active_queue_.pop_front();
    return p_r;
}

IOScheduler::IOScheduler()
{

}

class FIFOIOScheduler: public IOScheduler
{
    
};

class SSTFIOScheduler: public IOScheduler
{
    virtual IORequest* get_next_io_request();
};


// Shortest Seek Time First
IORequest* SSTFIOScheduler::get_next_io_request()
{
    debug("SSTFIOScheduler");
    int shortest_index = -1;
    int shortest_dist = std::numeric_limits<int>::max();;
    for(int i = 0; i< active_queue_.size(); i++) 
    {
        IORequest* p_r = active_queue_[i];
        int cur_dist = abs(p_r->track_ - kCurTrack);
        if(cur_dist < shortest_dist)
        {
            shortest_dist = cur_dist;
            shortest_index = i;
        }
        if(kDebugQueue) 
        {
            if(i == 0)
            {
                cout << "\t";
            }
            cout << p_r->rid_ << ":" << cur_dist << " ";
            if(i == active_queue_.size())
            {
                cout << endl;
            }
        }
    }
    if(kDebugQueue) 
    {
        cout << endl;
    }
    IORequest* p_r = active_queue_[shortest_index];
    auto it = active_queue_.begin()+shortest_index;
    active_queue_.erase(it);
    return p_r;
}


class LOOKIOScheduler: public IOScheduler
{
    public:
        virtual IORequest* get_next_io_request();
        void print_IORequest_Vectors(vector<IORequest*>& vec)
        {
            for(auto& r : vec) 
            {
                cout << r->rid_<< ":" << abs(r->track_ - kCurTrack)<< " ";
            }
        }
};





IORequest* LOOKIOScheduler::get_next_io_request()
{
    int lowest_high_index = -1;
    int highest_low_index = -1;
    int equal_index = -1;
    vector<IORequest*> high_vector;
    vector<IORequest*> low_vector;
    // if(kCurDirection == -2)
    // {
    //     kCurDirection = active_queue_.front().
    // }
    for(int i = 0; i< active_queue_.size(); i++) 
    {
        IORequest* p_r = active_queue_[i];
        if(p_r->track_ > kCurTrack)
        {
            if(lowest_high_index == -1)
            {
                lowest_high_index = i;
            }
            if(p_r->track_ < active_queue_[lowest_high_index]->track_)
            {
                lowest_high_index = i;
            }
            high_vector.emplace_back(p_r);
        }
        if(p_r->track_ < kCurTrack)
        {
            if(highest_low_index == -1)
            {
                highest_low_index = i;
            }
            if(p_r->track_ > active_queue_[highest_low_index]->track_)
            {
                highest_low_index = i;
            }
            low_vector.emplace_back(p_r);
        }
        if(p_r->track_ == kCurTrack)
        {
            equal_index = i;

            if(lowest_high_index == -1)
            {
                lowest_high_index = i;
            }
            if(p_r->track_ < active_queue_[lowest_high_index]->track_)
            {
                lowest_high_index = i;
            }
            high_vector.emplace_back(p_r);

            if(highest_low_index == -1)
            {
                highest_low_index = i;
            }
            if(p_r->track_ > active_queue_[highest_low_index]->track_)
            {
                highest_low_index = i;
            }
            low_vector.emplace_back(p_r);

        }
    }
    IORequest* next_io_request;
    bool changed = false;
    int index = -1;

    if(kCurDirection == -2)
    {
        int target_track = active_queue_[0]->track_;
        kCurDirection = (target_track > kCurTrack) ? 1 : -1;
    }

    if(kCurDirection == 1)
    {
        index = lowest_high_index;
        if(index == -1)
        {
            index = highest_low_index;
            changed = true;
        }
    }
    else
    {
        index = highest_low_index;
        if(index == -1)
        {
            index = lowest_high_index;
            changed = true;
        }
    }
    next_io_request = active_queue_[index];

    if(kDebugQueue)
    {
        int next_direction = (changed ? (-kCurDirection) : kCurDirection);
        debug(next_direction);
        auto& next_list = (next_direction == 1) ? high_vector : low_vector;
        next_io_request = (next_direction == 1) ? active_queue_[lowest_high_index] : active_queue_[highest_low_index];
        if(changed) 
        {
            cout << "\tGet: () --> change direction to "<< next_direction << endl;
        }
        cout << "\tGet: (";
        print_IORequest_Vectors(next_list);
        cout << ") --> " << next_io_request->rid_ << " dir=" << (next_direction) << endl;
    }
    auto it = active_queue_.begin()+index;
    active_queue_.erase(it);
    return next_io_request;
}


// Circular LOOK
class CLOOKIOScheduler: public LOOKIOScheduler
{
    public:
        virtual IORequest* get_next_io_request();
        bool is_same_direction(IORequest* r);
};

bool CLOOKIOScheduler::is_same_direction(IORequest* r)
{
    // if(r->track_ == kCurTrack)
    // {
    //     return true;
    // }
    if(r->track_ >= kCurTrack)
    {
        return true;
    }
    return false;
    // if(r->track_ < kCurTrack)
    // {
    //     return (kCurDirection == -1);
    // }
}

IORequest* CLOOKIOScheduler::get_next_io_request()
{
    int lowest_high_index = -1;
    int highest_low_index = -1;
    int equal_index = -1;
    
    // vector<IORequest*> high_vector;
    // vector<IORequest*> low_vector;
    // vector<vector<IORequest*> > vec;
    vector<IORequest*> same_direction_vec;
    vector<IORequest*> diff_direction_vec;

    // vctor<IORequest*> next_io_request_vector;

    // set init direction
    if(kCurDirection == -2)
    {
        int target_track = active_queue_[0]->track_;
        kCurDirection = (target_track > kCurTrack) ? 1 : -1;
    }

    int diff_direction_max_dist = -1;
    int diff_direction_max_dist_index = -1;

    int same_direction_min_dist = std::numeric_limits<int>::max();
    int same_direction_min_dist_index = -1;

    // for(int i = 0; i< active_queue_.size(); i++) 
    // {
    //     IORequest* p_r = active_queue_[i];
    //     if(p_r->track_ >= kCurTrack)
    //     {
    //         next_io_request_vector.emplace_back(p_r);
    //     }
    //     else
    //     {
    //         if(cur_dist > diff_direction_max_dist)
    //         {
    //             diff_direction_max_dist = cur_dist;
    //             diff_direction_max_dist_index = i;
    //         }

    //     }
    // }
    debug(kTime);
    debug(kCurTrack);
    for(int i = 0; i< active_queue_.size(); i++) 
    {
        IORequest* p_r = active_queue_[i];
        int cur_dist = abs(p_r->track_ - kCurTrack);
        if(is_same_direction(p_r))
        {
            same_direction_vec.emplace_back(p_r);
            if(cur_dist < same_direction_min_dist)
            {
                same_direction_min_dist = cur_dist;
                same_direction_min_dist_index = i;
            }
            if (p_r->track_ == 0) 
            {
                same_direction_min_dist = cur_dist;
                same_direction_min_dist_index = i;
            }
        }
        else
        {
            diff_direction_vec.emplace_back(p_r);
            if(cur_dist > diff_direction_max_dist)
            {
                diff_direction_max_dist = cur_dist;
                diff_direction_max_dist_index = i;
            }
        }
    }
    bool changed = false;
    int index = -1;
    IORequest* next_io_request;
    if(same_direction_min_dist_index == -1)
    {
        changed = true;
        index = diff_direction_max_dist_index;
    }
    else
    {
        index = same_direction_min_dist_index;
    }
    next_io_request = active_queue_[index];
    if(kDebugQueue)
    {
        int next_direction = (changed ? (-kCurDirection) : kCurDirection);
        // debug(next_direction);
        auto& next_list = (changed ? diff_direction_vec : same_direction_vec);
        cout << "\tGet: (";
        print_IORequest_Vectors(next_list);
        cout << ") --> " << next_io_request->rid_ << " dir=" << (next_direction) << endl;
    }


    auto it = active_queue_.begin()+index;
    active_queue_.erase(it);
    return next_io_request;
}


class FLOOKIOScheduler: public LOOKIOScheduler
{
    public:
        // deque<IORequest*> active_queue_;
        virtual void add_io_request(IORequest* p);
        virtual IORequest* get_next_io_request();
        virtual bool empty()
        {                
            return active_queue_.empty() && add_queue_.empty();
        }
};


void FLOOKIOScheduler::add_io_request(IORequest* p)
{
    add_queue_.emplace_back(p);
}


IORequest* FLOOKIOScheduler::get_next_io_request()
{
    if(active_queue_.empty()) 
    {
        active_queue_.swap(add_queue_);
    }
    IORequest* r = LOOKIOScheduler::get_next_io_request();
    return r;
}
