//
// Created by Arjun Krishnan on 2/24/25.
//

#include "memory.hpp"
#include <stdexcept>
#include <algorithm>

namespace drama {

    bool Allocation::size_compare = false;

    Memory::Memory(size_t l1_size, size_t l2_size, size_t l3_size, size_t size) :
        cache({
            .l1 = {
                    .block = new uint8_t[l1_size],
                    .size = l1_size,
            },

            .l2 = {
                    .block = new uint8_t[l2_size],
                    .size = l2_size,
            },

            .l3 = {
                    .block = new uint8_t[l3_size],
                    .size = l3_size,
            },
        }),
        bytes(new uint8_t[size]) {}

    Memory::~Memory() {
        delete []bytes;
        delete []cache.l1.block;
        delete []cache.l2.block;
        delete []cache.l3.block;
    }

    Block Memory::allocate_block(size_t block_size) {
        auto *begin = bytes + (alloc_map.empty() ? 0 : (alloc_map.rbegin()->offset + alloc_map.rbegin()->size));
        alloc_map.insert({
            reinterpret_cast<unsigned long long>(begin) - reinterpret_cast<unsigned long long>(bytes),
            block_size,
        });
        return {
                begin,
                block_size,
                this
        };
    }

    Block::Block(uint8_t *begin, size_t size, drama::Memory *parent) :
        m_begin(begin),
        m_size(size),
        m_parent(parent),
        m_free_map{ { .offset = 0, .size = size } } {}

    Block::~Block() {
        m_parent->alloc_map.erase({
            reinterpret_cast<unsigned long long>(m_begin) - reinterpret_cast<unsigned long long>(m_parent->bytes),
            m_size
        });
    }

    void Block::memset(size_t offset, const uint8_t *src, size_t n) {
        for (size_t i = 0; i < n; ++i)
            (m_begin + offset)[i] = src[i];
    }

    uint8_t *Block::load(size_t offset, size_t n) {
        if (auto *ptr = cache_attempt(offset, n))
            return ptr;

        // TODO: bounds checking
        return (m_begin + offset);
    }

    uint8_t *Block::cache_attempt(size_t offset, size_t n) {
        // TODO: implement
        return nullptr;
    }

    void Block::alloc(size_t size, bool stack) {
        if (stack)
            return stack_alloc(size);
        heap_alloc(size);
    }

    void Block::stack_alloc(size_t size) {
        m_sp += size;
    }

    void Block::heap_alloc(size_t size) {
        auto save = Allocation::size_compare;
        Allocation::size_compare = true;
        auto it = m_free_map.lower_bound({ .offset = 0, .size = size });
        if (it->size != size) {
            it = m_free_map.upper_bound({ .offset = 0, .size = size });
            if (it == m_free_map.end())
                throw std::runtime_error("Heap is full");
        }

        auto alloc_copy = *it;
        m_free_map.erase(it);
        alloc_copy.offset += size;

        m_free_map.insert(alloc_copy);
        m_alloc_map.insert({ .offset = alloc_copy.offset - size, .size = size });
        Allocation::size_compare = save;
    }

    void Block::stack_free(size_t size) {
        this->m_sp -= size;
    }

    void Block::heap_free(uint8_t *ptr) {
        auto save = Allocation::size_compare;
        auto it = m_alloc_map.find({ .offset = static_cast<size_t>(ptr - m_begin), .size = 0 });
        if (it != m_alloc_map.end()) {
            auto copy = *it;
            m_alloc_map.erase(it);

            Allocation::size_compare = false;
            auto l = m_free_map.lower_bound(copy);
            auto u = m_free_map.upper_bound(copy);
            if (l->offset + l->size == copy.offset) {
                auto l_copy = *l;
                m_free_map.erase(l);
                l_copy.size += copy.size;
                copy = l_copy;
            }

            if (copy.offset + copy.size == u->offset) {
                auto u_copy = *u;
                m_free_map.erase(copy);
                m_free_map.erase(u);
                copy.size += u_copy.size;
            }

            m_free_map.insert(copy);
        }

        Allocation::size_compare = save;
    }

    void Block::heap_free(size_t offset) {
        return heap_free(m_begin + offset);
    }

    void Block::load_program(drama::Executable file) {
        size_t data = file.data() - file.text(), text = file.text() - file.stack(), memory = file.heap() - file.stack();
        memcpy(m_begin, file.data(), data);
        memcpy(m_begin + data, file.text(), text);
        memcpy(m_begin + data + text, file.stack(), memory);
    }

} // drama