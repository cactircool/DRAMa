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

        J,
        JAL,

        AND,
        ANDI,

        OR,
        ORI,

        XOR,
        XORI,

        NOT,

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
        uint32_t registers[32];
        Memory *memory;
        std::stack<Block> blocks;

        static uint32_t consolidate(uint8_t *ptr, uint8_t size);
        static uint32_t trim(uint32_t a, uint8_t beg, uint8_t end);

        void interpret_text(Block &block, Executable &file);
        int interpret_text_unit(uint8_t *ptr); // returns instruction length to manage jumps

    public:
        CPU(size_t l1_size, size_t l2_size, size_t l3_size, size_t size);
        ~CPU();

        void execute(Executable file);
    };

} // drama

#endif //DRAMA_CPU_HPP
