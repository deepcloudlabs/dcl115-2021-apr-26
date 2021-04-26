#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

void fun(){
    int i; // stack ?
    cout << "Running fun by thread " << this_thread::get_id() << endl;
    cout << hex << &i << endl;
}

void gun(){
    vector<thread> threads;
    for (auto i=0;i<20;++i){
        threads.push_back(thread(fun));
    }
    /*
    for (auto &t: threads) {
        if (t.joinable())
            t.join();
    }
     */
    for_each(threads.begin(),threads.end(),mem_fn(&thread::join));
}

int main() {
    gun();
    cout << thread::hardware_concurrency()
         << " logical processors are available."
         << endl;
    return 0;
}