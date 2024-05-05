#include <IO/Protocol/RP2040_UART/Status.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    Status::Status() {
        throw Illegal("Status");
    }

    Status::Status(Node *node) {
        throw String_Exception("Not finished");
    }

    uint32_t Status::get_status() {
        return 0;
    }
}