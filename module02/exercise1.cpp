#include<iostream>
#include<thread>
#include <parallel/numeric>

using namespace std;

void fun(int x, int y) {
    cout << "x: " << x << endl;
    cout << "y: " << y << endl;
}

void gun(int *arr, int length) {
    for (int i = 0; i < length; ++i) {
        cout << "arr[" << i << "]: " << arr[i] << endl;
    }
}
void sun(int &result, int *arr_begin, int *arr_end) {
    result = __gnu_parallel::accumulate(arr_begin,arr_end,int (),[](int acc,int val){
        return acc + val;
    });
}

int main() {
    int a = 3, b = 5;
    int lost[]{4, 8, 15, 16, 23, 42};
    thread t1(fun, a, b);
    thread t2(gun, lost, 6);
    int sum;
    thread t3(sun, ref(sum),lost, lost+6);
    t1.join();
    t2.join();
    t3.join();
    cout << "sum: " << sum << endl;
    return 0;
}

