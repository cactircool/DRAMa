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

    void CPU::split(uint32_t a, uint32_t *offsets, uint32_t offsets_size, uint32_t *dst) {
        uint32_t offset_index = 0;
        uint32_t bit_index = 0;
        for (; bit_index < 32; ++bit_index) {
            if (bit_index == offsets[offset_index] && offsets_size > offset_index)
                ++offset_index;
            dst[offset_index] = (dst[offset_index] << 1) + ((a >> bit_index) & 1);
        }
    }

    void CPU::parse_type_r(uint32_t a, uint32_t *dst) {
        uint32_t part_offsets[] = { 6, 11, 16, 21, 26 };
        return split(a, part_offsets, 5, dst);
    }

    void CPU::parse_type_i(uint32_t a, uint32_t *dst) {
        uint32_t part_offsets[] = { 6, 11, 16 };
        return split(a, part_offsets, 3, dst);
    }

    void CPU::parse_type_j(uint32_t a, uint32_t *dst) {
        uint32_t part_offsets[] = { 6 };
        return split(a, part_offsets, 1, dst);
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
        switch (static_cast<ISA>(*ptr >> 2)) {
            case ISA::ADD: {
                uint32_t parts[5] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = static_cast<int32_t>(registers[parts[1]]) + static_cast<int32_t>(registers[parts[2]]);
                return 0;
            }
            case ISA::ADDU: {
                uint32_t parts[5] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = registers[parts[1]] + registers[parts[2]];
                return 0;
            }
            case ISA::ADDI: {
                uint32_t parts[4] = {0};
                parse_type_r(inst, parts);
                registers[parts[2]] = static_cast<int32_t>(registers[parts[1]]) + static_cast<int32_t>(parts[3]);
                return 0;
            }
            case ISA::ADDIU: {
                uint32_t parts[4] = {0};
                parse_type_r(inst, parts);
                registers[parts[2]] = registers[parts[1]] + parts[3];
                return 0;
            }
            case ISA::MUL: {
                uint32_t parts[5] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = static_cast<int32_t>(registers[parts[1]]) * static_cast<int32_t>(registers[parts[2]]);
                return 0;
            }
            case ISA::MULU: {
                uint32_t parts[5] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = registers[parts[1]] * registers[parts[2]];
                return 0;
            }
            case ISA::MULI: {
                uint32_t parts[4] = {0};
                parse_type_r(inst, parts);
                registers[parts[2]] = static_cast<int32_t>(registers[parts[1]]) * static_cast<int32_t>(parts[3]);
                return 0;
            }
            case ISA::MULIU: {
                uint32_t parts[4] = {0};
                parse_type_r(inst, parts);
                registers[parts[2]] = registers[parts[1]] * parts[3];
                return 0;
            }
            case ISA::DIV: {
                uint32_t parts[5] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = static_cast<int32_t>(registers[parts[1]]) / static_cast<int32_t>(registers[parts[2]]);
                return 0;
            }
            case ISA::DIVU: {
                uint32_t parts[5] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = registers[parts[1]] / registers[parts[2]];
                return 0;
            } 
            case ISA::DIVI: {
                uint32_t parts[4] = {0};
                parse_type_r(inst, parts);
                registers[parts[2]] = static_cast<int32_t>(registers[parts[1]]) / static_cast<int32_t>(parts[3]);
                return 0;
            }
            case ISA::DIVIU: {
                uint32_t parts[4] = {0};
                parse_type_r(inst, parts);
                registers[parts[2]] = registers[parts[1]] / parts[3];
                return 0;
            }
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