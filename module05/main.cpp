#include <iostream>
#include <atomic>
#include <vector>
#include <random>
#include <chrono>
#include <mutex>
#include <thread>

using namespace std;
class spinlock_mutex {
    atomic_flag flag;
public:
    spinlock_mutex() : flag(ATOMIC_FLAG_INIT) {}
    void lock(){
        while(flag.test_and_set(memory_order_acquire));
    }
    void unlock(){
        flag.clear(memory_order_release);
    }
};

vector<int> numbers;
mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
spinlock_mutex m;

void add_random_numbers(){
    lock_guard<spinlock_mutex> lockGuard(m);
    for (int i = 0; i < 10; ++i) {
        numbers.push_back(rng());
    }
    cout << this_thread::get_id() << endl;
}

int main() {

    thread t1(add_random_numbers);
    thread t2(add_random_numbers);
    thread t3(add_random_numbers);
    t1.join();
    t2.join();
    t3.join();
    cout << "size: " << numbers.size() << endl;
    for (auto &num : numbers) {
        cout << num << "," ;
    }
    cout << endl ;
    return 0;
}
