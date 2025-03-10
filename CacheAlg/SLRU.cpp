#include "SLRU.hpp"

SLRU::SLRU(size_t capacity, size_t protected_capacity)
{
    cap = capacity;
    protected_cap = protected_capacity;
}
    

void SLRU::access(int key) {
    if (cache_map.find(key) != cache_map.end()) {
        // Key is in cache
        access_count[key]++;
    } else if (protected_map.find(key) != protected_map.end()) {
        // Key is in protected
        access_count[key]++;
    } else {
        // Key is not in either list
        std::cout << "Key " << key << " not found." << std::endl;
    }
}

int SLRU::get(int key) {
    if (cache_map.find(key) != cache_map.end()) {
        access(key);
        return cache_map[key].first; // Return the value
    } else if (protected_map.find(key) != protected_map.end()) {
        access(key);
        return protected_map[key]; // Return the value from protected
    }
    return -1; // Return -1 if the key is not found
}

void SLRU::put(int key, int value) {
    if (cache_map.find(key) != cache_map.end()) {
        // Update existing key
        cache_map[key].first = value;
        access_count[key] = 1; // Reset access count
        access(key);
        return;
    }

    if (cache_list.size() >= cap) {
        evict_from_cache();
    }

    // Insert new key into cache
    cache_map[key] = {value, 1}; // value, initial access_time
    access_count[key] = 1; // Initialize access count
    cache_list.push_front(key);
}

void SLRU::move_to_protected(int key) {
    if (access_count[key] >= 2) {
        // Move to protected list
        protected_map[key] = cache_map[key].first; // Move value to protected
        cache_map.erase(key); // Remove from cache
        cache_list.remove(key); // Remove from cache list
        protected_list.push_front(key); // Add to protected list

        // Evict from protected if necessary
        if (protected_list.size() > protected_cap) {
            evict_from_protected();
        }
    }
}

void SLRU::evict_from_cache() {
    int lru_key = cache_list.back();
    if (access_count[lru_key] >= 2) {
        // Move to protected if accessed at least twice
        move_to_protected(lru_key);
    } else {
        // Otherwise, just remove it from cache
        cache_list.pop_back();
        cache_map.erase(lru_key);
        access_count.erase(lru_key); // Remove access count
    }
}

void SLRU::evict_from_protected() {
    int lru_key = protected_list.back();
    protected_list.pop_back();
    protected_map.erase(lru_key);
    access_count.erase(lru_key); // Remove access count
}

void SLRU::print_cache() {
    std::cout << "Cache List: ";
    for (const int key : cache_list) {
        std::cout << key << " -> " << cache_map.at(key).first << " ";
    }
    std::cout << std::endl;
}

void SLRU::print_protected() {
    std::cout << "Protected List: ";
    for (const int key : protected_list) {
        std::cout << key << " -> " << protected_map.at(key) << " ";
    }
    std::cout << std::endl;
}