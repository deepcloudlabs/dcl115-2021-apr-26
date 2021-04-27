#include <algorithm> 
#include <chrono> 
#include <iostream> 
#include <memory> 
#include <mutex> 
#include <string> 
#include <thread> 
#include <vector> 
#include <exception> 
 
using namespace std; 
 
class Chopstick{ 
public: 
    Chopstick(){}; 
    mutex m; 
}; 
 
int main(){ 
    auto eat = [](Chopstick* leftChopstick, Chopstick* rightChopstick, int philosopherNumber, int leftChopstickNumber, int rightChopstickNumber) 
    { 
        chrono::milliseconds timeout_begin(500); 
        this_thread::sleep_for(timeout_begin); 
        if (leftChopstick == rightChopstick) 
            throw string("Left and right chopsticks should not be the same!"); 
 
        lock(leftChopstick->m, rightChopstick->m); // deadlock-free
        lock_guard<mutex> a(leftChopstick->m, adopt_lock);
        string sl = "   Philosopher " + to_string(philosopherNumber) + " picked " + to_string(leftChopstickNumber) + " chopstick.\n"; 
        cout << sl.c_str(); 
 
        lock_guard<mutex> b(rightChopstick->m, adopt_lock);                     
        string sr = "   Philosopher " + to_string(philosopherNumber) + " picked " + to_string(rightChopstickNumber) + " chopstick.\n"; 
        cout << sr.c_str(); 
 
        string pe = "Philosopher " + to_string(philosopherNumber) + " eats.\n"; 
        cout << pe; 
 
        chrono::milliseconds timeout_end(500); 
        this_thread::sleep_for(timeout_end); 
    }; 
 
    static const int numPhilosophers = 5; 
 
    // 5 utencils on the left and right of each philosopher. Use them to acquire locks. 
    vector< unique_ptr<Chopstick> > chopsticks(numPhilosophers); 
 
    for (int i = 0; i < numPhilosophers; ++i) 
    { 
        auto c1 = unique_ptr<Chopstick>(new Chopstick()); 
        chopsticks[i] = move(c1); 
    } 
 
    // This is where we create philosophers, each of 5 tasks represents one philosopher. 
    vector<thread> tasks(numPhilosophers); 
 
    tasks[0] = thread(eat,  
            chopsticks[0].get(),                        // left chopstick:  #1 
            chopsticks[numPhilosophers - 1].get(),        // right chopstick: #5 
            0 + 1,                                        // philosopher number 
            1, 
            numPhilosophers 
        ); 
 
    for (int i = 1; i < numPhilosophers; ++i) 
    { 
        tasks[i] = (thread(eat,  
                chopsticks[i - 1].get(),                // left chopstick 
                chopsticks[i].get(),                    // right chopstick 
                i + 1,                                    // philosopher number 
                i, 
                i + 1 
                ) 
            ); 
    } 
 
    // May eat! 
    for_each(tasks.begin(), tasks.end(), mem_fn(&thread::join)); 
 
    return 0; 
} 