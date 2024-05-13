#ifndef RP2040_UART_H
#define RP2040_UART_H

#include <IO/Protocol/RP2040_UART/Data.h>
#include <IO/Protocol/RP2040_UART/Query.h>
#include <mutex>

namespace rgb_matrix {
    class RP2040_UART : public Protocol {
        public:
            RP2040_UART(Node *node, uint8_t magic = 0xAE);
            ~RP2040_UART();
        
        protected:
            RP2040_UART();

            Status internal_state_machine(bool clear_errors);

            Data *data_;
            Query *query_;
    };
}
#endif
