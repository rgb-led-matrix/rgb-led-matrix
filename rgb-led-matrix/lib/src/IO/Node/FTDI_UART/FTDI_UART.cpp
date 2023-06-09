#include <ftd2xx.h>
#include "IO/Node/FTDI_UART/FTDI_UART.h"

namespace rgb_matrix {
    FTDI_UART::FTDI_UART(const char *serial_number, uint8_t chan_num) {
        // TODO:
    }

    void FTDI_UART::write(char *buf, uint32_t len) {
        lock_.lock();

        // TODO:

        lock_.unlock();
    }

    int FTDI_UART::read(char **buf, uint32_t len, uint32_t timeout_us) {
        lock_.lock();

        // TODO:

        lock_.unlock();
    }
}