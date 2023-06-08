#ifndef FTDI_UART_H
#define FTDI_UART_H

#include "IO/Node/Node.h"

namespace rgb_matrix {
  class FTDI_UART : public Node {
    public:
        FTDI_UART(const char *serial_number, uint8_t chan_num);

        void write(char *buf, uint32_t len);
  };
}
#endif