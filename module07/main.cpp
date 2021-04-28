#include <atomic>
#include <thread>
#include <iostream>
#include <memory>
#include "domain.h"

using namespace std;

template <typename T>
class threadsafe_stack {
    struct node {
        shared_ptr<T> data;
        node *next;
        node(T const& data_):data(make_shared<T>(data_)){}
    };
    atomic<node*> head;
public:
    threadsafe_stack() {}

    void push(T const& data){ // write
        node* const new_node=new node(data);
        new_node->next=head.load();
        while(!head.compare_exchange_weak(new_node->next,new_node));
    }
    shared_ptr<T> pop(){ // read
        node* old_head=head.load();
        while(old_head &&
              !head.compare_exchange_weak(old_head,old_head->next));
        return old_head ? old_head->data : shared_ptr<T>();
    }

    bool empty(){
        return head.load() == nullptr;
    }
};

threadsafe_stack<programmer> tss;

void fun1(){
    tss.push(programmer(string("Jack"),string("Sheppard"),41,programming_language(string("c++"),8)));
    tss.push(programmer(string("Kate"),string("Austen"),36,programming_language(string("java"),6)));
    tss.push(programmer(string("Sun"),string("Kwon"),36,programming_language(string("scala"),5)));
    tss.push(programmer(string("Hugo"),string("Reyes"),29,programming_language(string("python"),7)));
    tss.push(programmer(string("John"),string("Lock"),29,programming_language(string("perl"),10)));
    tss.push(programmer(string("Claire"),string("Littleton"),27,programming_language(string("perl"),4)));
    tss.push(programmer(string("Juliet"),string("Burke"),36,programming_language(string("go"),9)));
    tss.push(programmer(string("Richard"),string("Alpert"),37,programming_language(string("f#"),9)));
    tss.push(programmer(string("Shannon"),string("Rutherford"),27,programming_language(string("pl/1"),2)));
    tss.push(programmer(string("Alex"),string("Rousseau"),21,programming_language(string("cobol"),2)));
}

void fun2(){
    tss.push(programmer(string("Ben"),string("Linus"),47,programming_language(string("c#"),9)));
    tss.push(programmer(string("James"),string("Ford"),47,programming_language(string("c"),6)));
    tss.push(programmer(string("Jin"),string("Kwon"),36,programming_language(string("lisp"),4)));
    tss.push(programmer(string("Desmond"),string("Hume"),38,programming_language(string("c#"),9)));
    tss.push(programmer(string("Charlie"),string("Pace"),32,programming_language(string("c"),6)));
    tss.push(programmer(string("Sayyid"),string("Jarrah"),41,programming_language(string("lisp"),4)));
    tss.push(programmer(string("Miles"),string("Leung"),32,programming_language(string("ada"),4)));
    tss.push(programmer(string("Walt"),string("Llyod"),16,programming_language(string("bash"),5)));
    tss.push(programmer(string("Daniel"),string("Rousseau"),49,programming_language(string("cobol"),7)));
}

void fun3(){
    tss.push(programmer(string("Daniel"),string("Faraday"),37,programming_language(string("c#"),9)));
    tss.push(programmer(string("Frank"),string("Lapidus"),47,programming_language(string("c"),6)));
    tss.push(programmer(string("Boone"),string("Carlyle"),36,programming_language(string("lisp"),4)));
    tss.push(programmer(string("Mr."),string("Eko"),38,programming_language(string("c#"),9)));
    tss.push(programmer(string("Charlie"),string("Pace"),32,programming_language(string("c"),6)));
    tss.push(programmer(string("Ana Lucia"),string("Cortez"),32,programming_language(string("lisp"),4)));
    tss.push(programmer(string("Bernard"),string("Nadler"),57,programming_language(string("ada"),4)));
    tss.push(programmer(string("Rose"),string("Nadler"),53,programming_language(string("bash"),5)));
    tss.push(programmer(string("Niki"),string("Fernandez"),32,programming_language(string("awk"),3)));
}
int main() {
    thread t1(fun1);
    thread t2(fun2);
    thread t3(fun3);
    t1.join();
    t2.join();
    t3.join();
    while (!tss.empty()){
        cout << *tss.pop() << endl;
    }
    return 0;
}
