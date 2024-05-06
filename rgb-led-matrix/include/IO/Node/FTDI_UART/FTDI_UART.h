#ifndef FTDI_UART_H
#define FTDI_UART_H

#include <string>
#include <IO/Node/Node.h>

namespace rgb_matrix {
    class FTDI_UART : public Node {
        public:
            FTDI_UART(const char *serial_number, uint8_t chan_num);

            // For Protocol
            void write(uint8_t *buf, uint32_t len);
            void read(uint8_t *buf, uint32_t *len, uint32_t timeout_us);

            // For Application (main)
            void set_baud(uint32_t baud);

        protected:
            FTDI_UART();
            
            std::string serial_number_;
    };
}
#endif