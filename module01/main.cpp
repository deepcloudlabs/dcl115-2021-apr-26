#include <iostream>
#include <thread> // not implementation => abstraction library

using namespace std;

// c++ -> new/delete -> linked list
void fun() { // escape analysis -> new int[1024] -> return/function parameter pass
    static int y=108;
    int numbers[1024]; // stack ?
    cout << "Have fun!" << endl;
}

struct task { // declaration
    int i; // shared memory -> stack/heap/data

    task(int i) : i(i) {}

    void operator()() {
        ++i; // write
        cout << "i: " << i << endl; // read // UB
    }
};

static int x = 42;

// Thread-safety : 1) Application 2) Data Structure -> Class/method
// 1st thread
int main() {
    int *pInt = new int (42);

    task my_task(42); // definition -> memory allocation -> stack

    thread t1(fun); // 1. function // 2nd thread
    thread t21(my_task); // 2. function object => functor // 3rd thread
    thread t22(my_task); // 2. function object => functor // 4th thread
    thread t3([](){ // 3. lambda expression (c++11) => functor // 5th thread
        cout << "Have fun!" << endl;
    });
    // cpu binding/affinity
    // cpu isolation: kernel parameter: 0,1 4,5: ultra-low latency
    // 10g NIC => device driver => 4
    // kernel distributes these threads to logical processors
    // ex: t1 -> cpu3, t2 -> cpu4, t3 -> cpu7
    // t1 -> has own stack but shares process:data/text/heap
    // t2 -> stack, t3 -> stack
    // stack size: ulimit -a -> stack size (kbytes, -s) 8192 -> 8M
    // 1024 thread -> Virtual Memory -> 8M * 1k -> 8GB
    // Process -> main ->   i) Text (method/function): fun, task::operator(), lambda expression
    //                     ii) Data: x, y
    //                    iii) Stack: my_task (object), pInt
    //                     iv) Heap:  new int (42)
    // stack -> local variable -> automatic variable -> destructor
    // RAII: resource acq. is initialization
    // Background Thread -> t1.detach()
    if (t1.joinable())
       t1.join(); // blocking method => i) sync. method ii) clean data structure
    t21.join();
    t22.join();
    t3.join();
    // t1 is done!
    delete pInt; // do not do this: delete pInt;
    return 0;
}
