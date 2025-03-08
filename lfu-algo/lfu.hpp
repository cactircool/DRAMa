#ifndef LFU_H
#define LFU_H

#include <cstdint>
#include <unordered_map>
#include <set>

namespace drama {
    // typedef std::pair<size_t, size_t> LFU_key;
    typedef size_t LFU_key;
    typedef int LFU_value;

    typedef std::pair<unsigned, LFU_key> LFU_freq;
    struct LFU_data {
        unsigned freq;
        LFU_value value;
    };
    
    // Optimal implementation of LFU using data structures
    class LFU {
        size_t max_size;
        std::unordered_map<LFU_key, LFU_data> lfu_mappings;
        std::set<LFU_freq> ordered_set;

        void update_key(LFU_key key, std::unordered_map<LFU_key, LFU_data>::iterator it);
        
    public:
        LFU(size_t max_size) : max_size(max_size) {}

        LFU_value get(LFU_key key);

        void put(LFU_key key, LFU_value value);
    };
}

#endif