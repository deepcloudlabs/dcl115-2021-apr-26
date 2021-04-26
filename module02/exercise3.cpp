#include <iostream>
#include <thread>
#include "thread_guard.h"

using namespace std;

void fun() {
    cout << "Running fun by thread " << this_thread::get_id() << endl;
}

int main() {
    // since C++20
    jthread t(fun);
    //thread_guard tg(t);
    //t.join();
    cout << "application is done." << endl;
    return 0;
}