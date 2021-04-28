#include <iostream>
#include <string>
#include <ranges>
#include <thread>
#include <mutex>

#include "../world/world-util.h"
#include "../world/city.h"
#include "../world/country.h"

using namespace std;
using namespace world;

map<int , shared_ptr<city>> cities;
map<string , shared_ptr<country>> countries;
int x=0; // Global -> Process Data -> Shared Data -> x!!!!
thread_local int y=0; // Stack ? Thread State -> NOT shared between threads!
void gun(){
   y++;  //
   x++;
}

void fun(){
    int y;
    for (int i=0;i<100;++i)
       gun();
    cout << y << endl; // 100
}

int main() { // this_thread::get_id() --> 1
    // threads -> n x readers -> x
    // threads -> m x writers -> x++
    thread t1(fun); // t1 -> Stack -> y, 2
    thread t2(fun); // t2 -> Stack -> y, 3
    cout << x << endl; // x -> 200
    cout << y << endl; // y -> 0
    t1.join();
    t2.join();
    create_world();
    auto is_asian = [](shared_ptr<country> & a_country) -> bool {
        return a_country->continent.compare("Asia") == 0;
    };
    auto asian_countries = countries | std::ranges::views::values
                                     | std::ranges::views::filter(is_asian);
    for (auto& a_country : asian_countries){
        cout << *a_country << endl;
    }

    for (auto& a_country : countries | std::ranges::views::values | std::ranges::views::filter(is_asian) ){
        cout << *a_country << endl;
    }
    return 0;
}
