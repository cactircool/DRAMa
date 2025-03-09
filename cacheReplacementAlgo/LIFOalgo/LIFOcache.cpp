#include "LIFOcache.hpp"
#include <iostream>

using namespace std;


int main()
{
    LIFOcache cache(3);


    cache.put(1,10);
    cache.put(2,20);
    cache.put(3,30);
    cache.printCache();

    cout << "value at key 2 is " << cache.get(2) << endl;

    cache.put(4,40);
    cache.printCache();

    return 1;
}