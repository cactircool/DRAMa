//
// Created by Arjun Krishnan on 2/24/25.
//

#include "cpu.hpp"

namespace drama {

    CPU::CPU(size_t l1_size, size_t l2_size, size_t l3_size, size_t size) :
            memory(new Memory(l1_size, l2_size, l3_size, size)),
            registers{0} {}

    uint32_t CPU::trim(uint32_t a, uint8_t beg, uint8_t end) {
        return (a >> (32 - end)) & ((1 << (end - beg + 1)) - 1);
    }

    void CPU::split(uint32_t a, const uint32_t *offsets, uint32_t offsets_size, uint32_t *dst) {
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

    uint32_t CPU::consolidate(const uint8_t *ptr, uint8_t size) {
        uint32_t result = 0;
        for (auto i = 0; i < size; ++i)
            result += ptr[i] << (i * 8);
        return result;
    }

    void CPU::syscall(drama::Block &block) {
        // Use registers[0,1,2] as the options

        switch (registers[0]) {
            case 1:
                block.alloc(registers[1], true);
                return;
            case 2:
                block.alloc(registers[1], false);
                return;
            case 3:
                block.stack_free(registers[1]);
                return;
            case 4:
                block.heap_free(registers[1]);
                return;
            default:
                return;
        }
    }

    void CPU::interpret_text_unit(Block &block, uint8_t *ptr, size_t &pc) {
        auto inst = consolidate(ptr, 4);
        switch (static_cast<ISA>(*ptr >> 2)) {
            case ISA::ADD: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = static_cast<int32_t>(registers[parts[1]]) + static_cast<int32_t>(registers[parts[2]]);
                return;
            }
            case ISA::ADDU: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = registers[parts[1]] + registers[parts[2]];
                return;
            }
            case ISA::ADDI: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                registers[parts[2]] = static_cast<int32_t>(registers[parts[1]]) + static_cast<int32_t>(parts[3]);
                return;
            }
            case ISA::ADDIU: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                registers[parts[2]] = registers[parts[1]] + parts[3];
                return;
            }
            case ISA::MUL: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = static_cast<int32_t>(registers[parts[1]]) * static_cast<int32_t>(registers[parts[2]]);
                return;
            }
            case ISA::MULU: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = registers[parts[1]] * registers[parts[2]];
                return;
            }
            case ISA::MULI: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                registers[parts[2]] = static_cast<int32_t>(registers[parts[1]]) * static_cast<int32_t>(parts[3]);
                return;
            }
            case ISA::MULIU: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                registers[parts[2]] = registers[parts[1]] * parts[3];
                return;
            }
            case ISA::DIV: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = static_cast<int32_t>(registers[parts[1]]) / static_cast<int32_t>(registers[parts[2]]);
                return;
            }
            case ISA::DIVU: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = registers[parts[1]] / registers[parts[2]];
                return;
            }
            case ISA::DIVI: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                registers[parts[2]] = static_cast<int32_t>(registers[parts[1]]) / static_cast<int32_t>(parts[3]);
                return;
            }
            case ISA::DIVIU: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                registers[parts[2]] = registers[parts[1]] / parts[3];
                return;
            }
            case ISA::BEQ: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                if (registers[parts[1]] == registers[parts[2]])
                    pc = pc + 4 + (parts[3] * 4);
                return;
            }
            case ISA::BNE: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                if (registers[parts[1]] != registers[parts[2]])
                    pc = pc + 4 + (parts[3] * 4);
                return;
            }
            case ISA::J: {
                uint32_t parts[2] = {0};
                parse_type_j(inst, parts);
                pc = parts[1];
                return;
            }
            case ISA::JAL: {
                uint32_t parts[2] = {0};
                parse_type_j(inst, parts);
                registers[31] = pc + 4;
                pc = parts[1];
                return;
            }
            case ISA::JR: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                pc = registers[parts[1]];
                return;
            }
            case ISA::AND: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = registers[parts[1]] & registers[parts[2]];
                return;
            }
            case ISA::ANDI: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                registers[parts[2]] = registers[parts[1]] & parts[3];
                return;
            }
            case ISA::OR: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = registers[parts[1]] | registers[parts[2]];
                return;
            }
            case ISA::ORI: {
                uint32_t parts[5] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = registers[parts[1]] | registers[parts[2]];
                return;
            }
            case ISA::XOR: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = registers[parts[1]] ^ registers[parts[2]];
                return;
            }
            case ISA::XORI: {
                uint32_t parts[5] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = registers[parts[1]] ^ registers[parts[2]];
                return;
            }
            case ISA::NOR: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = ~(registers[parts[1]] | registers[parts[2]]);
                return;
            }
            case ISA::LBU: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                registers[parts[2]] = *block.cache_attempt(registers[parts[1]] + parts[3], 1);
                return;
            }
            case ISA::LHU: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                auto *p = block.cache_attempt(registers[parts[1]] + parts[3], 2);
                registers[parts[2]] = consolidate(p, 2);
                return;
            }
            case ISA::LUI: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                registers[parts[2]] = parts[3] << 16;
                return;
            }
            case ISA::LW: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                auto *p = block.cache_attempt(registers[parts[1]] + parts[3], 4);
                registers[parts[2]] = consolidate(p, 4);
                return;
            }
            case ISA::SLT: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = static_cast<int32_t>(registers[parts[1]]) < static_cast<int32_t>(registers[parts[2]]) ? 1 : 0;
                return;
            }
            case ISA::SLTU: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = registers[parts[1]] < registers[parts[2]] ? 1 : 0;
                return;
            }
            case ISA::SLTI: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                registers[parts[2]] = static_cast<int32_t>(registers[parts[1]]) < static_cast<int32_t>(parts[3]) ? 1 : 0;
                return;
            }
            case ISA::SLTIU: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                registers[parts[2]] = registers[parts[1]] < parts[3] ? 1 : 0;
                return;
            }
            case ISA::SLL: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = registers[parts[2]] << parts[4];
            }
            case ISA::SRL: {
                uint32_t parts[6] = {0};
                parse_type_r(inst, parts);
                registers[parts[3]] = registers[parts[2]] >> parts[4];
            }
            case ISA::SB: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                block.memset(registers[parts[1]] + parts[3], reinterpret_cast<uint8_t *>(&registers[parts[2]]) + 3, 1);
                return;
            }
            case ISA::SHW: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                block.memset(registers[parts[1]] + parts[3], reinterpret_cast<uint8_t *>(&registers[parts[2]]) + 2, 2);
                return;
            }
            case ISA::SW: {
                uint32_t parts[4] = {0};
                parse_type_i(inst, parts);
                block.memset(registers[parts[1]] + parts[3], reinterpret_cast<uint8_t *>(&registers[parts[2]]), 4);
                return;
            }
            case ISA::SYSCALL: {
                syscall(block);
                return;
            }
            default:
                return;
        }
    }

    void CPU::interpret_text(drama::Block &block, drama::Executable &file) {
        for (size_t i = 0; i < file.stack() - file.text(); i += 4)
            interpret_text_unit(block, block.m_begin + (file.text() - file.data()) + i, i);
    }

    void CPU::execute(drama::Executable file) {
        blocks.emplace(memory->allocate_block(file.heap() - file.data()));
        interpret_text(blocks.top(), file);
    }

} // drama