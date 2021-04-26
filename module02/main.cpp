#include <iostream>
#include <thread>
#include "thread_guard.h"

using namespace std;


struct fun {
    int &i;

    fun(int &v) : i(v) {}

    void do_something() {
        cout << "Running do_something(): " << i << endl;
    }
    void operator()() {
        for (unsigned j = 0; j < 1000000; ++j)
            do_something();
    }
};

void gun() {
    int some_local_state = 0;
    fun my_fun(some_local_state);
    std::thread my_thread(my_fun);
    thread_guard tg(my_thread); // wrapper -> stack -> c++ -> destructor (return/throw)
    throw "ooops!";
}


int main() {
    gun();
    return 0;
}
