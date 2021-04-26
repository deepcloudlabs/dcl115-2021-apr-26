//
// Created by dcl on 4/26/2021.
//

#ifndef MODULE02_THREAD_GUARD_H
#define MODULE02_THREAD_GUARD_H


#include <thread>

class thread_guard {
    std::thread &t;
public:
    explicit thread_guard(std::thread &t);
    ~thread_guard(){
        if (t.joinable())
            t.join();
    }
    thread_guard(thread_guard const &) = delete;
    thread_guard& operator=(thread_guard const &) = delete;
};


#endif //MODULE02_THREAD_GUARD_H
