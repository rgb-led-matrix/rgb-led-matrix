#ifndef RP2040_UART_DATA_H
#define RP2040_UART_DATA_H

#include <stdint.h>
#include <IO/Node/Node.h>

namespace rgb_matrix {
    class Data {
        public:
            Data(Node *node);

            bool send_data(uint8_t *buf, uint32_t len);
        
        protected:
            Data();
    };
}
#endif
