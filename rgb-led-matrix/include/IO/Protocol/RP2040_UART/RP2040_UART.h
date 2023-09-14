#ifndef RP2040_UART_H
#define RP2040_UART_H

#include <stdint.h>
#include <IO/Protocol/Protocol.h>
#include <IO/Node/FTDI_UART/FTDI_UART.h>

namespace rgb_matrix {
    class RP2040_UART : public Protocol, FTDI_UART {
        public:
            void send(uint8_t *buf, uint32_t size, uint8_t chunk);
    };
}
#endif
