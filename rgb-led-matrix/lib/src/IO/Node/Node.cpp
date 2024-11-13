#include "IO/Node/Node.h"

namespace rgb_matrix {
    template <typename T> void Node::write_internal(T val) {
        uint8_t buf[sizeof(T)];

        for (uint32_t i = 0; i < sizeof(T); i++)
            buf[i] = (val & (1 << (i * 8))) & 0xFF;

        write(buf, sizeof(T));
    }

    void Node::write(uint32_t val) {
        write_internal<uint32_t>(val);
    }

    void Node::write(uint16_t val) {
        write_internal<uint16_t>(val);
    }

    void Node::write(uint8_t val) {
        write_internal<uint8_t>(val);
    }
}