#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <numeric>

using namespace std;

auto cpus = thread::hardware_concurrency();

template <typename Iterator, typename T>
void my_partial_sum(Iterator first,Iterator last,T &result){
    result = accumulate(first,last,result);
}

template <typename Iterator, typename T>
T parallel_sum(Iterator first,Iterator last){
    // data parallelism -> data partitioning
    // partition -> thread -> partial sum
    Iterator partition_start = first;
    auto partition_size = distance(first,last) / cpus;
    vector<thread> threads;
    vector<T> partial_sums(cpus);
    for (int i = 0; i < cpus; ++i) {
        Iterator partition_end = partition_start;
        advance(partition_end, partition_size);
        threads.push_back(
             thread(my_partial_sum<Iterator,T>,partition_start,partition_end, ref(partial_sums[i]))
        );
        partition_start = partition_end;
    }
    for_each(threads.begin(),threads.end(),mem_fn(&thread::join));
    return accumulate(partial_sums.begin(),partial_sums.end(),int());
}


int main() {
    int array[]{10, 20, 30, 40, 50, 60, 70, 80, 90, 10, 20, 30, 40, 50, 60, 70, 80, 90, 10, 20, 30, 40, 50, 60, 70, 80, 90, 10, 20, 30, 40, 50, 60, 70, 80, 90, 10, 20, 30, 40, 50, 60, 70, 80, 90, 10, 20, 30, 40, 50, 60, 70, 80, 90, 10, 20, 30, 40, 50, 60, 70, 80, 90, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    auto size = sizeof(array) / sizeof(int);
    auto result = parallel_sum<int*,int>(array,array+size);
    cout << "sum: " << result << endl;
    return 0;
}