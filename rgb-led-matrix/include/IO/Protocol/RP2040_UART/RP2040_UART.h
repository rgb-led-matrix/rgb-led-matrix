#ifndef RP2040_UART_H
#define RP2040_UART_H

#include <IO/Protocol/Protocol.h>

namespace rgb_matrix {
    class RP2040_UART : public Protocol {
        public:
            RP2040_UART(Node *node);

            void send(uint8_t *buf, uint32_t size);
            Protocol::Status get_protocol_status();
            void acknowledge(Protocol::Status);
        
        protected:
            RP2040_UART();
    };
}
#endif
