#include <iostream>
#include <algorithm>
#include <numeric>
#include <future>

using namespace std;

template <typename Iterator,typename Func>
void parallel_for_each(Iterator first, Iterator last,Func fun){
    unsigned long const length = distance(first,last);

    if (length == 0) return;

    if (length < 100){
        for_each(first,last,fun); // serial -> STL -> for_each
    } else {
        Iterator const mid_point = first+length / 2;
        future<void> first_half = async(&parallel_for_each<Iterator,Func>,first,mid_point,fun);
        parallel_for_each(mid_point,last,fun);
        first_half.get();
    }
}

template <typename Iterator,typename T, typename Func>
T parallel_accumulate(Iterator first, Iterator last,T initValue,Func fun){
    unsigned long const length = distance(first,last);

    if (length == 0) return initValue;

    if (length < 100){
        cout << this_thread::get_id() << endl;
        return accumulate(first,last,initValue, fun); // serial -> STL -> for_each
    } else {
        Iterator const mid_point = first+length / 2;
        future<T> first_half = async(&parallel_accumulate<Iterator,T,Func>,first,mid_point,initValue,fun);
        return first_half.get() + parallel_accumulate(mid_point,last,initValue,fun);
    }
}

int main() {
    vector<int> numbers;
    // main -> (LL) t1         (R) main
    //       (LL) t2 (LR) t1    (RL) t3 (RR) main
    // log2 1024 = 10 -> L + 1
    for (int i=0;i<1'000;++i) numbers.push_back(1);
    //parallel_for_each(numbers.begin(),numbers.end(),[](int i){cout << "i: " << i << endl; });
    int sum = parallel_accumulate(numbers.begin(),numbers.end(),int(),[](int sum,int val){return sum +val;});
    cout << "sum: " << sum << endl;
    return 0;
}
