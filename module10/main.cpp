#include <iostream>
#include <map>
#include <set>
#include <numeric>
#include <memory>
#include <execution>

#include "world/world-util.h"
#include "world/city.h"
#include "world/country.h"

using namespace std;
using namespace world;

map<int , shared_ptr<city>> cities;
map<string , shared_ptr<country>> countries;

int main() {
    create_world();
    // Find distinct continents
    set<string> continents;
    // 1. Outer Loop -> for/while/do-while
    for (auto& country : countries){
        // cout << country.first << ": " << country.second->name << endl;
        continents.insert(country.second->continent);
    }
    for (auto& continent : continents){
        cout << continent << endl;
    }
    // 2. Inner Loop -> Functional Programming -> One Liner
    auto continent_reducer = [](set<string> conts,shared_ptr<country> ulke){
        conts.insert(ulke->continent);
        return conts;
    };
    vector<shared_ptr<country>> ulkeler;

    for (auto const& [code, country] : countries){
        ulkeler.push_back(country);
    }
    if (ulkeler.size() > 1'000'000)
       sort(execution::par,ulkeler.begin(),ulkeler.end(),[](const shared_ptr<country> &c1,const shared_ptr<country>& c2){
           return c1->population > c2->population;
       });
    else
        sort(execution::seq,ulkeler.begin(),ulkeler.end(),[](const shared_ptr<country> &c1,const shared_ptr<country>& c2){
            return c1->population > c2->population;
        });
    continents = accumulate(ulkeler.begin(),ulkeler.end(),set<string>(),continent_reducer);
    for_each(continents.begin(),continents.end(),[](const string &continent){cout << continent << endl;});
    return 0;
}
