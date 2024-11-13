#include "IO/Protocol/RP2040_UART/Data_Protocol/Status_msg.h"
#include "IO/Protocol/RP2040_UART/internal.h"
#include "IO/machine.h"

namespace rgb_matrix::Protocol::RP2040_UART {
    uint32_t Status_msg::compute_checksum() {
        uint32_t checksum = 0xFFFFFFFF;

        checksum = internal::checksum_chunk(checksum, header, 32);
        checksum = internal::checksum_chunk(checksum, cmd, 8);
        checksum = internal::checksum_chunk(checksum, len, 16);
        checksum = internal::checksum_chunk(checksum, status, 32);

        return ~checksum;
    }

    bool Status_msg::valid(uint8_t magic) {
        if (ntohl(header) != internal::generate_header(magic))
            return false;

        if (cmd != 's')
            return false;

        if (ntohs(len) != 4)
            return false;

        if (ntohl(delimiter) != internal::generate_delimiter(magic))
            return false;

        if (ntohl(checksum) != compute_checksum())
            return false;

        return true;
    }
}