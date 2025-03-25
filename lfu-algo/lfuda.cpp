#include "lfuda.hpp"
#include <iostream>

using namespace drama;

namespace drama {
    void LFUDA::update_key(LFU_key key, std::unordered_map<LFU_key, LFUDA_data>::iterator it) {
        // Increment frequency
        ordered_set.erase({it->second.age, it->second.freq, key});
        it->second.freq++;
        ordered_set.emplace(it->second.age, it->second.freq, key);
    }

    LFU_value LFUDA::get(LFU_key key) {
        auto it = lfu_mappings.find(key);
        // In reality, fetch data from memory and put into cache
        // Right now, return -1 if the key is not in cache
        if (it == lfu_mappings.end())
            return -1;
        update_key(key, it);
        return it->second.value;
    }

    void LFUDA::put(LFU_key key, LFU_value val) {
        auto it = lfu_mappings.find(key);
        // If the key is already in the map
        if (it != lfu_mappings.end()) {
            it->second.value = val;
            update_key(key, it);
        }
        else {
            // Get rid of LFU if full
            while (lfu_mappings.size() >= max_size) {
                auto remove_it = --ordered_set.end();
                lfu_mappings.erase(remove_it->key);
                ordered_set.erase(remove_it);
            }
            unsigned age = curr_age++;
            lfu_mappings[key] = {age, 0, val};
            ordered_set.emplace(age, 0, key);
        }
    }
}

int main() {
    LFUDA cache(3);
    cache.put(1, 2);
    cache.put(2, 5);
    cache.put(3, 11);
    for (int i = 0; i < 4; i++)
        cache.get(1);
    std::cout << cache.get(1) << '\n';
    std::cout << cache.get(2) << '\n';
    std::cout << cache.get(3) << '\n';
    for (int i = 0; i < 3; i++)
        cache.put(i + 4, i * 2 + 1);
    // 4 5 6 print 1 3 5
    // Dynamic aging currently cares about age
    // significantly more than frequency
    std::cout << cache.get(1) << '\n';
    std::cout << cache.get(2) << '\n';
    std::cout << cache.get(3) << '\n';
    std::cout << cache.get(4) << '\n';
    std::cout << cache.get(5) << '\n';
    std::cout << cache.get(6) << '\n';
}