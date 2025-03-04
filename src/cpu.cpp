//
// Created by Arjun Krishnan on 2/24/25.
//

#include "cpu.hpp"

namespace drama {

    CPU::CPU(size_t l1_size, size_t l2_size, size_t l3_size, size_t size) :
        memory(new Memory(l1_size, l2_size, l3_size, size)) {}

    uint32_t CPU::trim(uint32_t a, uint8_t beg, uint8_t end) {
        return (a >> (32 - end)) & ((1 << (end - beg + 1)) - 1);
    }

    CPU::~CPU() {
        delete memory;
    }

    uint32_t CPU::consolidate(uint8_t *ptr, uint8_t size) {
        uint32_t result = 0;
        for (auto i = 0; i < size; ++i)
            result += ptr[i] << (i * 8);
        return result;
    }

    int CPU::interpret_text_unit(uint8_t *ptr) {
        // TODO: implement
        auto inst = consolidate(ptr, 4);
        switch (static_cast<ISA>(*ptr >> 3)) {
            case ISA::ADD:
                
                return 0;
            case ISA::ADDU:
                return 0;
            case ISA::ADDI:
                return 0;
            case ISA::ADDIU:
                return 0;
            case ISA::MUL:
                return 0;
            case ISA::MULU:
                return 0;
            case ISA::MULI:
                return 0;
            case ISA::MULIU:
                return 0;
            case ISA::DIV:
                return 0;
            case ISA::DIVU:
                return 0;
            case ISA::DIVI:
                return 0;
            case ISA::DIVIU:
                return 0;
            case ISA::BEQ:
                return 0;
            case ISA::J:
                return 0;
            case ISA::JAL:
                return 0;
            case ISA::AND:
                return 0;
            case ISA::ANDI:
                return 0;
            case ISA::OR:
                return 0;
            case ISA::ORI:
                return 0;
            case ISA::XOR:
                return 0;
            case ISA::XORI:
                return 0;
            case ISA::NOT:
                return 0;
            case ISA::LBU:
                return 0;
            case ISA::LHU:
                return 0;
            case ISA::LUI:
                return 0;
            case ISA::LW:
                return 0;
            case ISA::SLT:
                return 0;
            case ISA::SLTU:
                return 0;
            case ISA::SLTI:
                return 0;
            case ISA::SLTIU:
                return 0;
            case ISA::SLL:
                return 0;
            case ISA::SRL:
                return 0;
            case ISA::SB:
                return 0;
            case ISA::SHW:
                return 0;
            case ISA::SW:
                return 0;
            case ISA::SYSCALL:
                return 0;
            default:
                return 1;
        }
    }

    void CPU::interpret_text(drama::Block &block, drama::Executable &file) {
        for (size_t i = 0; i < file.stack() - file.text();) 
            i += interpret_text_unit(block.m_begin + (file.text() - file.data()) + i);
    }

    void CPU::execute(drama::Executable file) {
        blocks.emplace(memory->allocate_block(file.heap() - file.data()));
        interpret_text(blocks.top(), file);
    }

} // drama