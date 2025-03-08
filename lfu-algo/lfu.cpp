#include "lfu.hpp"
#include <iostream>

using namespace drama;

namespace drama {
    void LFU::update_key(LFU_key key, std::unordered_map<LFU_key, LFU_data>::iterator it) {
        // Increment frequency
        ordered_set.erase({it->second.freq, key});
        it->second.freq++;
        ordered_set.emplace(it->second.freq, key);
    }

    LFU_value LFU::get(LFU_key key) {
        auto it = lfu_mappings.find(key);
        // In reality, fetch data from memory and put into cache
        // Right now, return -1 if the key is not in cache
        if (it == lfu_mappings.end())
            return -1;
        update_key(key, it);
        return it->second.value;
    }

    void LFU::put(LFU_key key, LFU_value val) {
        auto it = lfu_mappings.find(key);
        // If the key is already in the map
        if (it != lfu_mappings.end()) {
            it->second.value = val;
            update_key(key, it);
        }
        else {
            // Get rid of LFU if full
            while (lfu_mappings.size() >= max_size) {
                auto remove_it = ordered_set.begin();
                lfu_mappings.erase(remove_it->second);
                ordered_set.erase(remove_it);
            }
            lfu_mappings[key] = {0, val};
            ordered_set.emplace(0, key);
        }
    }

}

int main() {
    LFU cache(3);
    cache.put(1, 2);
    cache.put(2, 5);
    cache.put(3, 11);
    std::cout << "2 = " << cache.get(1) << '\n';
    std::cout << "11 = " << cache.get(3) << '\n';
    cache.get(3);
    cache.put(4, 23); // 2 is removed
    std::cout << "23 = " << cache.get(4) << '\n'; // 23
    std::cout << "-1 = " << cache.get(2) << '\n'; // -1
    std::cout << "11 = " << cache.get(3) << '\n'; // 11
    cache.put(5, 1); // 1 is removed
    std::cout << "-1 = " << cache.get(1) << '\n'; // -1
    cache.put(6, 2);
    std::cout << "23 = " << cache.get(4) << '\n';
    std::cout << "-1 = " << cache.get(5) << '\n'; // -1
    std::cout << "2 = " << cache.get(6) << '\n';
}