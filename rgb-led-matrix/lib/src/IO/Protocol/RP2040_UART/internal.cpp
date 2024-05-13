#include <IO/Protocol/RP2040_UART/internal.h>

namespace rgb_matrix {
    uint32_t internal::generate_header(uint8_t magic) {
        // TODO:
        return 0xAAEEAEE;
    }

    uint32_t internal::generate_delimiter(uint8_t magic) {
        // TODO:
        return 0xAEAEAEAE;
    }
}