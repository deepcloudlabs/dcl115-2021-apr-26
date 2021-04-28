#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;
#pragma once

template<typename T>
class thread_safe_queue {
		mutex mut;
		queue<T> data_queue;
		condition_variable data_cond;
	public:
		thread_safe_queue(){}
		thread_safe_queue(const thread_safe_queue&) = delete ;
		thread_safe_queue& operator=(const thread_safe_queue&) = delete;
		void push(T new_value);
		bool try_pop(T& value);
		T pop();
		bool empty() const {
		   lock_guard<mutex> lk(mut);
		   return data_queue.empty();
		}
};