//
// Created by Arjun Krishnan on 2/24/25.
//

#include "executable.hpp"

namespace drama {

    Executable::Executable(uint8_t *data, uint8_t *text, uint8_t *stack, uint8_t *heap) :
        m_data(data),
        m_text(text),
        m_stack(stack),
        m_heap(heap) {}

    Executable::~Executable() = default;

} // drama