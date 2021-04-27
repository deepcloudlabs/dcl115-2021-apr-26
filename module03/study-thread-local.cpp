#include <vector>
#include <iostream>
#include <thread>
#include <algorithm>

using namespace std;

thread_local vector<int> numbers; // thread stack -> thread-safe

void doWork(int i){
    numbers.push_back(i);
    for_each(numbers.begin(),numbers.end(),[](int i){cout << i << endl;});
}

void doAnotherWork(int i){
    numbers.push_back(i);
    for_each(numbers.begin(),numbers.end(),[](int i){cout << i << endl;});
}

int main() {
    numbers.push_back(5);
    thread t1([](){
        for (int i = 0; i < 10; ++i) {
            doWork(i);
            doAnotherWork(i);
        }});
    thread t2(doWork,2);
    t1.join(); t2.join();
    for_each(numbers.begin(),numbers.end(),[](int i){cout << i << endl;});
    return 0;
}
