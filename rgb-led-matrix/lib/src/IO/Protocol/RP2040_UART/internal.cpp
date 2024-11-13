#include "IO/Protocol/RP2040_UART/internal.h"
#include "IO/CRC/CRC.h"

namespace rgb_matrix::Protocol::RP2040_UART {
    uint32_t internal::generate_header(uint8_t magic) {
        uint32_t upper = magic >> 4;
        uint32_t lower = magic & 0xF;
        uint32_t result = upper << 12 | upper << 8 | lower << 4 | lower;
        return result << 16 | result;
    }

    uint32_t internal::generate_delimiter(uint8_t magic) {
        uint32_t upper = magic >> 4;
        uint32_t lower = magic & 0xF;
        uint32_t result = upper << 4 | lower;
        return result << 24 | result << 16 | result << 8 | result;
    }

    uint32_t internal::checksum_chunk(uint32_t checksum, uint32_t v, uint8_t bits) {
        for (int i = 0; i < bits; i += 8)
            checksum = CRC::crc32(checksum, (v >> i) & 0xFF);
        
        return checksum;
    }
}