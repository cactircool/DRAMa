#ifndef LFU_H
#define LFU_H

#include "lfuutil.hpp"
#include <unordered_map>
#include <set>
using namespace drama;

namespace drama {
    // Optimal implementation of LFU using data structures
    class LFU {
        size_t max_size;
        std::unordered_map<LFU_key, LFU_data> lfu_mappings;
        std::set<LFU_freq> ordered_set;

        void update_key(LFU_key key, std::unordered_map<LFU_key, LFU_data>::iterator it);
        
    public:
        LFU(size_t max_size) : max_size(max_size) {}

        LFU_value get(LFU_key key);

        void put(LFU_key key, LFU_value val);
    };
}

#endif