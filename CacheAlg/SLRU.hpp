#ifndef SLRU_HPP
#define SLRU_HPP

#include <list>
#include <unordered_map>
#include <utility>
#include <iostream>
using namespace std;

class SLRU {
public:
    SLRU(size_t capacity, size_t protected_capacity);
    void access(int key);
    int get(int key);
    void put(int key, int value);
    void print_cache();
    void print_protected();

private:
    void move_to_protected(int key);
    void evict_from_cache();
    void evict_from_protected();

    size_t cap; 
    size_t protected_cap; 
    list<int> cache_list;
    list<int> protected_list; 
    unordered_map<int, pair<int, int>> cache_map; 
    unordered_map<int, int> protected_map; 
    unordered_map<int, int> access_count; 
};

#endif 