#include "LILOcache.hpp"

#include <iostream>

using namespace std;

int main()
{
    LILO cache(3);

    cache.put(1,10);  
    cache.put(2,20); 
    cache.put(3,30); 
    cache.printCache();

    cout << "Value at key 1: " <<cache.get(1) << endl; 


    cache.put(5,40); 
    cache.printCache();
}