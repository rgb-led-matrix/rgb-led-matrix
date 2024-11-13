#ifndef RP2040_UART_DATA_PROTOCOL_DATA_H
#define RP2040_UART_DATA_PROTOCOL_DATA_H

#include <stdint.h>
#include "IO/Protocol/Data_Protocol.h"
#include "IO/Protocol/RP2040_UART/Data_Protocol/Status.h"
#include "IO/Protocol/RP2040_UART/Data_Protocol/Data_Operation/Data_Command.h"

namespace rgb_matrix::Protocol::RP2040_UART {
    class Data {
        public:
            Data(Node *node, uint8_t magic);
            ~Data();

            Data_Protocol::Status send_data(void *buf, uint32_t len, uint8_t sizeof_t, uint8_t multiplex, uint8_t columns, Data_Format_ID format);
            void clear_errors();
        
        protected:
            Data();

            Data_Command *runnable_;
    };
}
#endif
