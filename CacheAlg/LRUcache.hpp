#ifndef LLcache_hpp
#define LLcache_hpp

#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

class LRUcache
{
    private:
        list<pair<int,int>> cache_list;
        unordered_map <int, list<pair<int,int>>::iterator> cache_map;
        int capacity;
    
    // Declare function 
    public:
        LRUcache(int capacity);
        int get(int key);
        void put(int key, int value);   
        void print();
};




#endif