#ifndef RP2040_UART_H
#define RP2040_UART_H

#include <IO/Protocol/Protocol.h>
#include <mutex>

namespace rgb_matrix {
    class RP2040_UART : public Protocol {
        public:
            RP2040_UART(Node *node);
        
        protected:
            RP2040_UART();

            Status internal_state_machine();

            std::mutex lock_;
    };
}
#endif
