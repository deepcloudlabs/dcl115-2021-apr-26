#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>

using namespace std;

struct barrier { // lock-free => multicore
    atomic<int> counter[2];
    atomic<int> lock[2];
    atomic<int> cur_idx;
    int parties;

    barrier(int parties) : parties(parties) {
    }

    bool await() {
        int idx = cur_idx.load();
        if (lock[idx].load() == 0) {
            lock[idx].store(1);
        }
        int val = counter[idx].fetch_add(1);
        if (val >= parties - 1) {
            counter[idx].store(0);
            cur_idx.fetch_xor(1);
            lock[idx].store(0);
            return true;
        }
        while (lock[idx].load() == 1);
        return false;
    }
};

mutex m;

barrier bar(4);

void fun() {
    bar.await();
    m.lock();
    cout << "I am here " << this_thread::get_id() << endl;
    m.unlock();
}

int main() {
    thread t1(fun);
    thread t2(fun);
    thread t3(fun);
    thread t4(fun);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
