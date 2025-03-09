#ifndef LFU_UTIL
#define LFU_UTIL

#include <cstdint>
#include <utility>

namespace drama {
    // typedef std::pair<size_t, size_t> LFU_key;
    typedef size_t LFU_key;
    typedef int LFU_value;
    typedef std::pair<unsigned, LFU_key> LFU_freq;
    
    struct LFU_data {
        unsigned freq;
        LFU_value value;
    };
    typedef std::pair<LFU_key, LFU_value> LFU_pair;
}

#endif