#include <string>
#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

class some_data { // thread-safe class -> live-ness
    int a;
    int b;
    mutex m1;
    mutex m2;
public:
    some_data(int a, int b) : a(a), b(b) {}
    void do_something(){ // t1
        lock_guard<mutex> lockGuard(m1);
        a++; // writer
    }
    void do_other(){ // t2
        lock_guard<mutex> lockGuard(m2);
        b++; // writer
    }
    void do_yet_another(){ // Thread-safe -> Thread Live-ness -> DeadLock-free
        lock(m1,m2); // DeadLock-free
        lock_guard<mutex> lockGuard1(m1);
        lock_guard<mutex> lockGuard2(m2);
        a++;
        b++;
    }
};

int main() {
    some_data someData(42,108);
    thread t1([&someData](){
        for (int i = 0; i < 100; ++i) {
            someData.do_something();
            someData.do_other();
        }
    });
    thread t2([&someData](){
        for (int i = 0; i < 100; ++i) {
            someData.do_something();
            someData.do_other();
        }
    });
    t1.join();
    t2.join();
    return 0;
}