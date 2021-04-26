#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <mutex>

using namespace std;

static int x = 0; // global data -> shared memory
mutex m;

void fun() {
    m.lock(); // mutual exclusion -> serialization
    for (int i = 0; i < 100'000; i++) {
        // race condition -> critical section -> mutual exclusion -> locking
        x++; // Write -> increment -> it is NOT atomic
    }
    m.unlock();
}

int main() {
    vector<thread> threads;
    for (int i = 0; i < 8; ++i) {
        threads.push_back(thread(fun));
    }
    for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
    cout << "data: " << x << endl; // 1'000'000
    return 0;
}
