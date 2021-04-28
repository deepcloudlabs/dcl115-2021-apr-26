#include "threadsafe_queue.h"
#include <mutex>
#include <memory>

using namespace std;

template<typename T>
void thread_safe_queue<T>::push(T new_value){
	lock_guard<mutex> lk(mut);
	data_queue.push(new_value);
	data_cond.notify_one();
}

template<typename T>
bool thread_safe_queue<T>::try_pop(T& value){
	unique_lock<mutex> lk(mut);
	if (!data_queue.empty()){
		value=data_queue.front();
		data_queue.pop();		
		return true;
	}
	return false;
}

template<typename T>
T thread_safe_queue<T>::pop(){
	unique_lock<mutex> lk(mut);
	data_cond.wait(lk,[this]{return !data_queue.empty();});
    T value= data_queue.front();	
	data_queue.pop();		
	return value;
}
