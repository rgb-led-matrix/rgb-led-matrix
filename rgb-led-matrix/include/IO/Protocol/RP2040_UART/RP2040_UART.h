#ifndef RP2040_UART_H
#define RP2040_UART_H

#include <stdint.h>
#include <IO/Protocol/Protocol.h>
#include <IO/Node/Node.h>

namespace rgb_matrix {
    class RP2040_UART : public Protocol {
        public:
            void send(uint8_t *buf, uint32_t size, Node *node);
            Protocol::Status get_protocol_status();
    };
}
#endif
