#include "lfru.hpp"
using namespace drama;

#include <iostream>

LFU_value LFRU::get(LFU_key key) {
    auto it = unprivileged.find(key);
    if (it != unprivileged.end()) {
        it->second.freq++;
        if (it->second.freq >= popular_threshold) {
            // Push to privileged if popular
            privileged.emplace_back(it->first, it->second.value);
            unprivileged.erase(it);
            return privileged.back().second;
        }
        return it->second.value;
    }
    auto list_it = privileged.begin();
    for (; list_it != privileged.end(); list_it++) {
        // std::cout << "Key: " << key << ", privileged element: " << list_it->first << '\n';
        if (list_it->first == key) {
            // Update the use order of privileged LRU cache if key is found, then return
            privileged.splice(privileged.end(), privileged, list_it);
            return privileged.back().second;
        }
    }
    // Return -1 if key was not found
    return -1;
}

void LFRU::put(LFU_key key, LFU_value val) {
    auto list_it = privileged.begin();
    for (; list_it != privileged.end(); list_it++) {
        if (list_it->first == key) {
            // Update the use order of privileged LRU cache if key is found, and update the value
            privileged.splice(privileged.end(), privileged, list_it);
            privileged.back().second = val;
            return;
        }
    }
    auto it = unprivileged.find(key);
    if (it != unprivileged.end()) {
        it->second.freq++;
        if (it->second.freq >= popular_threshold) {
            // Push to privileged if popular
            privileged.emplace_back(it->first, it->second.value);
            privileged.back().second = val;
            unprivileged.erase(it);
            return;
        }
        it->second.value = val;
        return;
    }
    // If cold miss, add the key to the cache
    if (unprivileged.size() + privileged.size() >= max_size) {
        if (unprivileged.size()) {
            // Remove LFU from unprivileged section if full
            auto least_used = unprivileged.begin();
            for (auto it = ++unprivileged.begin(); it != unprivileged.end(); it++) {
                if (it->second.freq < least_used->second.freq)
                    least_used = it;
            }
            unprivileged.erase(least_used);
        }
        else {
            // Alternatively remove LRU from privileged section if unprivileged is empty
            privileged.pop_front();
        }
    }
    unprivileged[key] = {0, val};
}

void LFRU::print() {
    std::cout << "Unprivileged:\n";
    for (auto x : unprivileged)
        std::cout << '(' << x.first << '=' << x.second.freq << ',' << x.second.value << ") ";
    std::cout << "\nPrivileged:\n";
    for (auto x : privileged)
        std::cout << '(' << x.first << '=' << x.second << ") ";
    std::cout << '\n';
}

int main() {
    LFRU cache(3, 2);
    cache.put(1, 2);
    cache.put(2, 5);
    cache.put(3, 11);
    std::cout << "2 = " << cache.get(1) << '\n';
    std::cout << "11 = " << cache.get(3) << '\n';
    cache.get(3);
    // Unprivileged: 2; Privileged: 1, 3
    cache.put(4, 23); // 2 is removed
    std::cout << "23 = " << cache.get(4) << '\n'; // 23
    std::cout << "-1 = " << cache.get(2) << '\n'; // -1
    std::cout << "11 = " << cache.get(3) << '\n'; // 11
    cache.get(4);
    cache.put(5, 1); // 1 is removed
    std::cout << "-1 = " << cache.get(1) << '\n'; // -1
    cache.get(5);
    cache.get(5);
    // Unprivileged: ; Privileged: 3, 4, 5
    cache.put(6, 2); // 3 is removed
    // Unprivileged: 6; Privileged: 4, 5
    std::cout << "-1 = " << cache.get(3) << '\n';
    std::cout << "1 = " << cache.get(5) << '\n'; // -1
    std::cout << "2 = " << cache.get(6) << '\n';
}