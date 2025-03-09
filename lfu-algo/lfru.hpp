#ifndef LFRU_H
#define LFRU_H

#include "lfuutil.hpp"
#include <list>
#include <unordered_map>
using namespace drama;

namespace drama {
    // Linear-time Implementation of LFRU using data structures
    class LFRU {
        size_t max_size;
        size_t popular_threshold;
        std::unordered_map<LFU_key, LFU_data> unprivileged;
        std::list<LFU_pair> privileged;

    public:
        LFRU(size_t max_size, size_t popular_threshold = 3) : max_size(max_size), popular_threshold(popular_threshold) {}
        
        LFU_value get(LFU_key key);

        void put(LFU_key key, LFU_value val);

        void print();
    };
}

#endif