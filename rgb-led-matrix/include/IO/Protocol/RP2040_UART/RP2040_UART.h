#ifndef RP2040_UART_H
#define RP2040_UART_H

#include <stdint.h>

namespace rgb_matrix {
    class RP2040_UART {
        public:
            void send(uint8_t *buf, uint32_t size, uint8_t chunk);
    };
}
#endif
