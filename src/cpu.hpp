//
// Created by Arjun Krishnan on 2/24/25.
//

#ifndef DRAMA_CPU_HPP
#define DRAMA_CPU_HPP

#include <cstdint>
#include <stack>
#include "executable.hpp"
#include "memory.hpp"

namespace drama {

    enum class ISA : uint8_t {
        ADD,
        ADDU,
        ADDI,
        ADDIU,

        MUL,
        MULU,
        MULI,
        MULIU,

        DIV,
        DIVU,
        DIVI,
        DIVIU,

        BEQ,
        BNE,

        J,
        JAL,
        JR,

        AND,
        ANDI,

        OR,
        ORI,

        XOR,
        XORI,

        NOR,

        LBU,
        LHU,
        LUI,
        LW,

        SLT,
        SLTU,
        SLTI,
        SLTIU,

        SLL,
        SRL,

        SB,
        SHW,
        SW,

        SYSCALL,
    };

    class CPU {
        uint32_t registers[32]; // reg32 = ra
        Memory *memory;
        std::stack<Block> blocks;

        static uint32_t consolidate(const uint8_t *ptr, uint8_t size);
        static void split(uint32_t a, const uint32_t *offsets, uint32_t offsets_size, uint32_t *dst);
        static uint32_t trim(uint32_t a, uint8_t beg, uint8_t end);

        static void parse_type_r(uint32_t a, uint32_t *dst);
        static void parse_type_i(uint32_t a, uint32_t *dst);
        static void parse_type_j(uint32_t a, uint32_t *dst);

        void interpret_text(Block &block, Executable &file);
        void interpret_text_unit(Block &block, uint8_t *ptr, size_t &pc);

        void syscall(Block &block);

    public:
        CPU(size_t l1_size, size_t l2_size, size_t l3_size, size_t size);
        ~CPU();

        void execute(Executable file);
    };

} // drama

#endif //DRAMA_CPU_HPP