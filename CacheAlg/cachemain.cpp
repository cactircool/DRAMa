#include <iostream>
#include <unordered_map>
#include "LRUcache.cpp"
using namespace std;

int main()
{
    LRUcache cache = LRUcache(4);
    cache.put(1,1);
    cache.put(2,2);
    cache.put(3,10);
    cache.put(2,5);
    
    int temp = cache.get(3);
    
    cache.print();
    cout << (temp);

    
    return 0;
}




//LRU and SLRU and PLRU and benchmark(research about)