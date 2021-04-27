#include <string>
#include <list>
#include <queue>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>

using namespace std;

struct product {
    string name;
    int quantity;
    double price;
    double sub_total;

    product(const string &name, int quantity, double price) : name(name), quantity(quantity),
                                                              price(price), sub_total(price * quantity) {}
};

struct purchase_order {
    list<product> items;
    double total;

    purchase_order(const list<product> &items) : items(items) {
        total = 0.0;
    }
};

mutex m;
queue<purchase_order> orders; //shared memory
condition_variable order_ready;

void receive_po(list<purchase_order> &orders) {
    for (auto order : orders) {
        lock_guard<mutex> lockGuard(m);
        cout << "Sending order " << this_thread::get_id() << " ..." << endl;
        ::orders.push(order); // shared memory
        order_ready.notify_one(); // synchronization
    }
}

void process_po() {
    unique_lock<mutex> uniqueLock(m);
    order_ready.wait(uniqueLock, [] { return !orders.empty(); });
    auto po = orders.front();
    orders.pop();
    uniqueLock.unlock();
    for (auto item : po.items) {
        cout << this_thread::get_id() << " -> " << item.name << endl;
    }
}

int main() {
    list<purchase_order> my_orders1;
    list<purchase_order> my_orders2;
    list<product> items1, items2, items3;
    items1.push_back(product("AAA", 1, 10.0));
    items1.push_back(product("BBB", 2, 20.0));
    items2.push_back(product("CCC", 3, 30.0));
    items2.push_back(product("DDD", 4, 40.0));
    items3.push_back(product("EEE", 5, 50.0));
    items3.push_back(product("FFF", 6, 60.0));
    my_orders1.push_back(purchase_order(items1));
    my_orders1.push_back(purchase_order(items2));
    my_orders2.push_back(purchase_order(items3));

    thread producer1(receive_po, ref(my_orders1)); // 2
    thread producer2(receive_po, ref(my_orders2)); // 3
    thread consumer1(process_po); // 4
    thread consumer2(process_po); // 5
    thread consumer3(process_po); // 6
    producer1.join();
    producer2.join();
    consumer1.join();
    consumer2.join();
    consumer3.join();
    cout << "Application is done." << endl;
    return 0;
}