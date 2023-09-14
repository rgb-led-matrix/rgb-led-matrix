#ifndef FTDI_UART_H
#define FTDI_UART_H

#include <mutex>
#include <string>
#include "IO/Node/Node.h"

namespace rgb_matrix {
  class FTDI_UART : public Node {
    public:
      FTDI_UART(const char *serial_number, uint8_t chan_num);

      void set_baud(uint32_t baud);
      void send(uint8_t *buf, uint32_t size);
      bool process(uint8_t stages);

    protected:
      void write(char *buf, uint32_t len);
      int read(char **buf, uint32_t len, uint32_t timeout_us);

      std::mutex lock_;
      std::string serial_number_;
      uint8_t *buf_;
      uint32_t size_;
      uint32_t counter_;
      uint8_t stage_;
  };
}
#endif