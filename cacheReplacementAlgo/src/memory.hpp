//
// Created by Arjun Krishnan on 2/24/25.
//

#ifndef DRAMA_MEMORY_HPP
#define DRAMA_MEMORY_HPP

#include <cstdint>
#include <unordered_set>
#include <set>
#include "executable.hpp"
#include "../LILOalgo/LILOcache.hpp"
#include "../LIFOalgo/LIFOcache.hpp"
#include "../FIFOalgo/FIFOcache.hpp"
#include "../FILOaglo/FILOcache.hpp"
#include "../SIEVE/SIEVE.hpp"

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
        struct Cache {
            LILO* lilo_cache;
            LIFOcache* lifo_cache;
            LIFOcache* filo_cache;  // FILO uses same data structure as LIFO
            Sieve* sieve_cache;
            
            // Cache levels with their blocks and sizes
            struct {
                uint8_t *block;
                size_t size;
            } l1;

            struct {
                uint8_t *block;
                size_t size;
            } l2;

            struct {
                uint8_t *block;
                size_t size;
            } l3;

            // Initialize caches with appropriate sizes
            void init_caches() {
                // LILO/FIFO algorithm for L1 cache
                lilo_cache = new LILO(l1.size);
                
                // LIFO algorithm for L2 cache
                lifo_cache = new LIFOcache(l2.size);
                
                // FILO algorithm uses LIFO data structure with L2 cache
                filo_cache = new LIFOcache(l2.size);
                
                // SIEVE algorithm for L3 cache
                sieve_cache = new Sieve(l3.size);
            }

            // Clean up caches
            void cleanup() {
                delete lilo_cache;
                delete lifo_cache;
                delete filo_cache;
                delete sieve_cache;
            }

            Cache() : lilo_cache(nullptr), lifo_cache(nullptr), filo_cache(nullptr), sieve_cache(nullptr) {}
            ~Cache() { cleanup(); }
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
