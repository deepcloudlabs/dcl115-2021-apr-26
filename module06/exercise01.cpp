#include <thread>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

#include "domain.h"

using namespace std;

template<typename T>
class threadsafe_queue {
private:
    mutable mutex mut;
    queue<T> data_queue;
    condition_variable data_cond;
public:
    threadsafe_queue(){}

    void push(T new_value){
        lock_guard<mutex> lk(mut);
        data_queue.push(move(new_value));
        data_cond.notify_one();
    }

    void wait_and_pop(T& value){
        unique_lock<mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=move(data_queue.front());
        data_queue.pop();
    }

    shared_ptr<T> wait_and_pop(){
        unique_lock<mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        shared_ptr<T> res(
                make_shared<T>(move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool try_pop(T& value){
        lock_guard<mutex> lk(mut);
        if(data_queue.empty())
            return false;
        value=move(data_queue.front());
        data_queue.pop();
    }

    shared_ptr<T> try_pop(){
        lock_guard<mutex> lk(mut);
        if(data_queue.empty())
            return shared_ptr<T>();
        shared_ptr<T> res(
                make_shared<T>(move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool empty() const {
        lock_guard<mutex> lk(mut);
        return data_queue.empty();
    }

    int size() const {
        lock_guard<mutex> lk(mut);
        return data_queue.size();
    }
};

threadsafe_queue<programmer> ts_queue;

void fun1(){
    ts_queue.push(programmer(string("Jack"),string("Sheppard"),41,programming_language(string("c++"),8)));
    ts_queue.push(programmer(string("Kate"),string("Austen"),36,programming_language(string("java"),6)));
    ts_queue.push(programmer(string("Sun"),string("Kwon"),36,programming_language(string("scala"),5)));
    ts_queue.push(programmer(string("Hugo"),string("Reyes"),29,programming_language(string("python"),7)));
    ts_queue.push(programmer(string("John"),string("Lock"),29,programming_language(string("perl"),10)));
    ts_queue.push(programmer(string("Claire"),string("Littleton"),27,programming_language(string("perl"),4)));
    ts_queue.push(programmer(string("Juliet"),string("Burke"),36,programming_language(string("go"),9)));
    ts_queue.push(programmer(string("Richard"),string("Alpert"),37,programming_language(string("f#"),9)));
    ts_queue.push(programmer(string("Shannon"),string("Rutherford"),27,programming_language(string("pl/1"),2)));
    ts_queue.push(programmer(string("Alex"),string("Rousseau"),21,programming_language(string("cobol"),2)));
}

void fun2(){
    ts_queue.push(programmer(string("Ben"),string("Linus"),47,programming_language(string("c#"),9)));
    ts_queue.push(programmer(string("James"),string("Ford"),47,programming_language(string("c"),6)));
    ts_queue.push(programmer(string("Jin"),string("Kwon"),36,programming_language(string("lisp"),4)));
    ts_queue.push(programmer(string("Desmond"),string("Hume"),38,programming_language(string("c#"),9)));
    ts_queue.push(programmer(string("Charlie"),string("Pace"),32,programming_language(string("c"),6)));
    ts_queue.push(programmer(string("Sayyid"),string("Jarrah"),41,programming_language(string("lisp"),4)));
    ts_queue.push(programmer(string("Miles"),string("Leung"),32,programming_language(string("ada"),4)));
    ts_queue.push(programmer(string("Walt"),string("Llyod"),16,programming_language(string("bash"),5)));
    ts_queue.push(programmer(string("Daniel"),string("Rousseau"),49,programming_language(string("cobol"),7)));
}

void fun3(){
    ts_queue.push(programmer(string("Daniel"),string("Faraday"),37,programming_language(string("c#"),9)));
    ts_queue.push(programmer(string("Frank"),string("Lapidus"),47,programming_language(string("c"),6)));
    ts_queue.push(programmer(string("Boone"),string("Carlyle"),36,programming_language(string("lisp"),4)));
    ts_queue.push(programmer(string("Mr."),string("Eko"),38,programming_language(string("c#"),9)));
    ts_queue.push(programmer(string("Charlie"),string("Pace"),32,programming_language(string("c"),6)));
    ts_queue.push(programmer(string("Ana Lucia"),string("Cortez"),32,programming_language(string("lisp"),4)));
    ts_queue.push(programmer(string("Bernard"),string("Nadler"),57,programming_language(string("ada"),4)));
    ts_queue.push(programmer(string("Rose"),string("Nadler"),53,programming_language(string("bash"),5)));
    ts_queue.push(programmer(string("Niki"),string("Fernandez"),32,programming_language(string("awk"),3)));
}

int main(){
    thread t1(fun1);
    thread t2(fun2);
    thread t3(fun3);
    t1.join();
    t2.join();
    t3.join();
    cout << "There are " << ts_queue.size() << " programmers in the stack!" << endl;
    while (!ts_queue.empty()){
        cout << *ts_queue.wait_and_pop() << endl ;
    }
    return 0;

}
