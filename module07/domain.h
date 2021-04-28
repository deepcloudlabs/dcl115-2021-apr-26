#include <string>
#include <ostream>

using namespace std;

#pragma once

struct programming_language {
    string name;
    int level;
    programming_language(string name,int level) : name(name), level(level) {
    }
    programming_language(const programming_language& p) =default;

    programming_language(){}
    friend ostream& operator<<(ostream& out,const programming_language& pl){
        out << "programming_language [name= " << pl.name << ", level=" << pl.level << "]" ;
        return out;
    }
};

struct programmer {
    string first_name;
    string last_name;
    int age;
    struct programming_language pl;
    programmer(){}
    programmer(string first_name,string last_name, int age,programming_language pl) :
            first_name(first_name), last_name(last_name), pl(pl)
    {
    }
    friend ostream& operator<<(ostream& out,const programmer& programmer){
        out << "programmer [first_name= "
            << programmer.first_name
            << ", last_name= "
            << programmer.last_name
            << ", "
            << programmer.pl
            << "]" ;
        return out;
    }
    programmer(const programmer& p) =default;
    programmer(programmer&& p) : first_name(std::move(p.first_name)),last_name(std::move(p.last_name)),age(p.age) {
    }

} ;
