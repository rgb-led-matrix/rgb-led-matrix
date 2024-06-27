#ifndef RP2040_UART_DATA_H
#define RP2040_UART_DATA_H

#include <IO/Protocol/Data_Protocol.h>
#include <IO/Protocol/Control_Protocol.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    class Data;
    class Query;
    class Control;
    
    class RP2040_UART_Data : public Data_Protocol {
        public:
            RP2040_UART_Data(Node *node, uint8_t id, uint8_t magic = 0xAE);
            ~RP2040_UART_Data();

            float get_temperature(Control_Protocol *protocol);
        
        protected:
            RP2040_UART_Data();

            Data_Protocol::Status internal_state_machine(bool clear_errors);

            Data *data_;
            Query *query_;
    };
}
#endif
