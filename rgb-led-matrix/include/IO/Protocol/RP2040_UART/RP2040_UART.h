#ifndef RP2040_UART_H
#define RP2040_UART_H

#include <IO/Protocol/Protocol.h>

namespace rgb_matrix {
    class Data;
    class Query;
    class Control;
    
    class RP2040_UART : public Protocol {
        public:
            RP2040_UART(Node *data, Node *control, uint8_t magic = 0xAE);
            ~RP2040_UART();
        
        protected:
            RP2040_UART();

            Status internal_state_machine(bool clear_errors);
            void signal(Commands command);

            Data *data_;
            Query *query_;
            Control *control_;
    };
}
#endif
