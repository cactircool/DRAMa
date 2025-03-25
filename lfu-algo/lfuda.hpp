#ifndef LFU_H
#define LFU_H

#include "lfuutil.hpp"
#include <unordered_map>
#include <set>
using namespace drama;

namespace drama {
    struct LFUDA_data {
        unsigned age, freq;
        LFU_value value;
    };

    struct LFUDA_freq {
        long long weight;
        unsigned age, freq;
        LFU_key key;
        LFUDA_freq(unsigned age, unsigned freq, LFU_key key) : weight((long long) freq - age * 3), age(age), freq(freq), key(key) {}
        LFUDA_freq() : LFUDA_freq(0, 0, LFU_key()) {}
    };
    bool operator<(const LFUDA_freq &a, const LFUDA_freq &b) {
        if (a.weight == b.weight)
            return a.key < b.key;
        return a.weight < b.weight;
    }

    // Implementation of LFUDA using data structures
    class LFUDA {
        unsigned curr_age;
        size_t max_size;
        std::unordered_map<LFU_key, LFUDA_data> lfu_mappings;
        std::set<LFUDA_freq> ordered_set;

        void update_key(LFU_key key, std::unordered_map<LFU_key, LFUDA_data>::iterator it);
        
    public:
        LFUDA(size_t max_size) : curr_age(0), max_size(max_size) {}

        LFU_value get(LFU_key key);

        void put(LFU_key key, LFU_value val);
    };
}

#endif