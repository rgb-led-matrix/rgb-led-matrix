#ifndef RP2040_UART_CONTROL_H
#define RP2040_UART_CONTROL_H

#include "IO/Protocol/Control_Protocol.h"

namespace rgb_matrix::Protocol::RP2040_UART {
    class Data;
    class Query;
    class Control;
    
    class RP2040_UART_Control : public Control_Protocol {
        public:
            RP2040_UART_Control(Node *node, uint8_t magic = 0xAE);
            ~RP2040_UART_Control();
        
        protected:
            RP2040_UART_Control();

            void internal_signal(Commands command);

            Control *control_;
    };
}
#endif
