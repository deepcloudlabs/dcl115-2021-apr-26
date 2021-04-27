#include <thread>
#include <iostream>
#include <future>


long _3n_plus_1(long n){
    long step = 0;
    while (n>1){
        if (n%2 == 1){
            n = 3 * n + 1;
        } else {
            n = n / 2;
        }
        step++;
    }
    return step;
}

int main() {
    std::future<long> step = std::async(_3n_plus_1, 17);
    std::cout << "17 -> " << step.get() << std::endl;
    return 0;
}