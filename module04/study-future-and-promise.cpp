#include <iostream>
#include <future>
#include <functional>

using namespace std;

long _3n_plus_1(future<long>& ft){
    long n = ft.get();
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
    promise<long> number;
    future<long> n = number.get_future();
    future<long> step = async(_3n_plus_1, ref(n));
    number.set_value(17); // triggers line # 8
    std::cout << "17 -> " << step.get() << std::endl;
    return 0;
}