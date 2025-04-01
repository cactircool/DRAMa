//
// Created by Arjun Krishnan on 2/24/25.
//

#ifndef DRAMA_EXECUTABLE_HPP
#define DRAMA_EXECUTABLE_HPP

#include <cstdint>

namespace drama {

    class Executable {
        uint8_t *m_data, *m_text, *m_stack, *m_heap;
    public:
        Executable(uint8_t *data, uint8_t *text, uint8_t *stack, uint8_t *heap);
        ~Executable();

        uint8_t *data() const { return m_data; }
        uint8_t *&data() { return m_data; }

        uint8_t *text() const { return m_text; }
        uint8_t *&text() { return m_text; }

        uint8_t *stack() const { return m_stack; }
        uint8_t *&stack() { return m_stack; }

        uint8_t *heap() const { return m_heap; }
        uint8_t *&heap() { return m_heap; }
    };

} // drama

#endif //DRAMA_EXECUTABLE_HPP
