#include <iostream>
#include <functional>
#include <thread>
#include <atomic>
#include <map>
#include "threadsafe_queue.cpp"

using namespace std;

class thread_pool {
    thread_safe_queue<function<void()>> work_queue;
    vector<thread> threads;
    map<thread::id,int> counter;
    atomic_bool done;
    int thread_count = thread::hardware_concurrency();

    void worker_thread() {
        counter[this_thread::get_id()] = 0;
        while (!done) {
            function<void()> task;
            if (work_queue.try_pop(task)) {
                task();
                counter[this_thread::get_id()]++;
                cerr << this_thread::get_id() << ": " << counter[this_thread::get_id()] << endl;
            }
                this_thread::yield();

        }
    }

public:
    thread_pool() : done(false) {
        for (int i = 0; i < thread_count; ++i) {
            threads.push_back(
                    thread(&thread_pool::worker_thread, this)
            );
        }
    }

    template<typename FunctionType>
    void submit(FunctionType task) {
        work_queue.push(function<void()>(task));
    }

    void shutdown() {
        done.store(true);
        for (auto &t : threads) {
            if (t.joinable())
                t.join();
        }
    }

    ~thread_pool() {
        shutdown();
    }
};
mutex m;

void print(string message){
    lock_guard<mutex> lockGuard(m);
    cerr << message << endl;
}

void fun() {
    lock_guard<mutex> lockGuard(m);
    cerr << this_thread::get_id() << " - Hello Mars!" << endl;
}

void gun() {
    lock_guard<mutex> lockGuard(m);
    cerr << this_thread::get_id() << " - Hello Moon!" << endl;
}

void sun() {
    //lock_guard<mutex> lockGuard(m);
    cerr << this_thread::get_id() << " - Hello Sun!" << endl;
}

int main() {
    thread_pool my_pool;
    for (int i = 0; i < 1000; ++i) {
        my_pool.submit(fun);
        my_pool.submit(gun);
        my_pool.submit(sun);
    }
    //my_pool.shutdown();
    int i;
    cin >> i;
    cout << "Application is done." << endl;
    return 0;
}
