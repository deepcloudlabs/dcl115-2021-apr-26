#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;
using namespace chrono;
bool flag = false;
mutex m;

void fun(int duration) {
    cout << this_thread::get_id() << " is entering..." << endl;
    this_thread::sleep_for(milliseconds(duration)); // parking
    flag = true;
    cout << this_thread::get_id() << " is leaving..." << endl;
}

void wait_for_flag(){
    unique_lock<mutex> uniqueLock(m);
    while (!flag){
        uniqueLock.unlock();
        this_thread::sleep_for(milliseconds(100));
        uniqueLock.lock();
    }
    cout << this_thread::get_id() << " is working now..." << endl;
}

int main() {
    thread t1(fun, 3000);
    thread t2(wait_for_flag);
    t1.join();
    t2.join();
    return 0;
}
