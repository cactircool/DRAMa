#ifndef SLRU_HPP
#define SLRU_HPP

#include <list>
#include <unordered_map>
#include <utility>
#include <iostream>

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

    size_t cap; // Capacity of the cache list
    size_t protected_cap; // Capacity of the protected list
    std::list<int> cache_list; // List for cache
    std::list<int> protected_list; // List for protected items
    std::unordered_map<int, std::pair<int, int>> cache_map; // Maps key to (value, access_time)
    std::unordered_map<int, int> protected_map; // Maps key to value
    std::unordered_map<int, int> access_count; // New map to track access times
};

#endif // SLRU_HPP