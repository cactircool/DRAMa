//
// Created by Arjun Krishnan on 2/24/25.
//

#ifndef DRAMA_MEMORY_HPP
#define DRAMA_MEMORY_HPP

#include <cstdint>
#include <unordered_set>
#include <set>
#include "executable.hpp"
#include "LRUcache.hpp"
#include "SLRU.hpp"


namespace drama {

    class Memory;
    class CPU;

    struct Allocation {
        size_t offset;
        size_t size;

        static bool size_compare;

        bool operator<(const Allocation &other) const {
            if (size_compare)
                return size < other.size;
            return offset < other.offset;
        }
    };

    class Block {
        uint8_t *m_begin;
        uint8_t *m_sp;
        size_t m_size;
        Memory *m_parent;

        std::set<Allocation> m_alloc_map;
        std::set<Allocation> m_free_map;

        void stack_alloc(size_t size);
        void heap_alloc(size_t size);

        #define f(x) uint8_t *cache_##x(size_t offset, size_t n);

        f(lfu)
        f(lfru)
        f(lfuda)
        f(lifo)
        f(lilo) 
        f(filo) 
        f(fifo) 
        f(sieve)
        f(lru)
        f(slru)

        #undef f

    public:
        Block(uint8_t *begin, size_t size, Memory *parent);
        ~Block();

        void load_program(Executable file);

        void stack_free(size_t size);
        void heap_free(uint8_t *ptr);
        void heap_free(size_t offset);
        void alloc(size_t size, bool stack = true);

        uint8_t *cache_attempt(size_t offset, size_t n);

        // This is where cache replacement will happen
        uint8_t *load(size_t offset, size_t n);
        void memset(size_t offset, const uint8_t *src, size_t n);

        friend CPU;
    };

    class Memory {
        struct LRU_SLRU {
            LRUcache l1;
            SLRU l2;

            LRU_SLRU(size_t l1_size, size_t l2_size, size_t l2_protected_size)
            : l1(l1_size), l2(l2_size,l2_protected_size){}

        } cache;

        uint8_t *bytes; 
        size_t size;
        std::set<Allocation> alloc_map;

    public:
        Memory(size_t l1_size, size_t l2_size, size_t l3_size, size_t size);
        ~Memory();

        Block allocate_block(size_t block_size);
        friend Block;
    };

} // drama

#endif //DRAMA_MEMORY_HPP
