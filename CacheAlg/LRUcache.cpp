#include "LRUcache.hpp"
#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;


// Get the size for the list

LRUcache::LRUcache(int capacity)
{
    this -> capacity = capacity;
}

int LRUcache::get(int key)
{

    // if key does not exist then return to exit out

    if (cache_map.find(key) == cache_map.end())
    {
        return -1;
    }   
    
    cache_list.splice(cache_list.begin(), cache_list, cache_map[key]);
    return cache_map[key] -> second;
}

void LRUcache::put(int key, int value)
{
    // Get the value for the assign key
    
    auto it = cache_map.find(key);

    // Update the value for the assign key if it already exist and move to top

    if (it != cache_map.end())
    {
        it->second->second = value;
        cache_list.splice(cache_list.begin(), cache_list, cache_map[key]);
        return;
    }

    // If the capacity reach the maximum
    
    if (cache_list.size() == capacity)
    {
        int temp = cache_list.back().first;
        cache_list.pop_back();
        cache_map.erase(temp);
    }

    // If it has not been add and still space left

    cache_list.push_front({key,value});
    cache_map[key] = cache_list.begin();

}


void LRUcache::print()
{
    for (auto it = cache_list.begin(); it != cache_list.end(); it++)
    {   
        cout << it ->first << " -> " << it -> second << endl;
    }
}