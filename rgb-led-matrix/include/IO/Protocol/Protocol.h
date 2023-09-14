#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

namespace rgb_matrix {
    class Node;
    
    class Protocol {
        public:
            virtual ~Protocol() {}

            virtual void send(uint8_t *buf, uint32_t size, uint8_t chunk, Node *node) = 0;
    };
}
#endif
