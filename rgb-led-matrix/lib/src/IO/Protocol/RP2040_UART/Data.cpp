#include <IO/Protocol/RP2040_UART/Data.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    Data::Data() {
        throw Illegal("Status");
    }

    Data::Data(Node *node) {
        throw String_Exception("Not finished");
    }

    bool Data::send_data(uint8_t *buf, uint32_t length) {
        return false;
    }
}