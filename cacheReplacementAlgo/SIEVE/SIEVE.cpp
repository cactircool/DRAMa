#include "SIEVE.hpp"

#include <iostream>

using namespace std;

int main() 
{
    Sieve cache(3);

    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);
    cache.printCache();

    cout << "Get key 2: " << cache.get(2) << endl;
    cache.printCache();
    cache.put(4, 40);
    cache.printCache();
    cache.put(5,50);
    cache.printCache();

    cout << "Get key 4: " << cache.get(4) << endl;
    cache.put(6,60);
    cache.printCache();

    return 0;
}