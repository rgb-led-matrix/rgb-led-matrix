#ifndef RP2040_UART_DATA_PROTOCOL_STATUS_MSG_H
#define RP2040_UART_DATA_PROTOCOL_STATUS_MSG_H

#include <stdint.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    struct Status_msg {
        uint32_t header;
        uint8_t cmd;
        uint16_t len;
        uint32_t status;
        uint32_t checksum;
        uint32_t delimiter;

        bool valid(uint8_t magic);
        uint32_t compute_checksum();
    };
}
#endif
