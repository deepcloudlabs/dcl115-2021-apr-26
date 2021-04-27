#include <iostream>
#include <memory>
#include <thread>
#include <mutex>

using namespace std;

class my_resource {
    int i;
public:
    my_resource(int i) : i(i) {}
    void do_something(){
        cout << "i: " << i << endl;
    }
};

shared_ptr<my_resource> resource_ptr; // global data -> shared memory

once_flag resource_flag;

void init_resource(){ // once
    cout << "resource initialized!" << endl;
    resource_ptr.reset(new my_resource(42));
}

void fun(){
    call_once(resource_flag, init_resource); // resource (shared memory) initialization
    //init_resource();
    resource_ptr->do_something();
}

int main() {
    thread t1(fun);
    thread t2(fun);
    thread t3(fun);
    t1.join(); t2.join(); t3.join();
    return 0;
}
