#include <IO/Protocol/RP2040_UART/Data.h>
#include <IO/Protocol/Protocol.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    Data::Data() {
        throw Illegal("Status");
    }

    Data::Data(Node *node) {
        throw String_Exception("Not finished");

        status_ = new Status(node);
    }

    Protocol::Status Data::send_data(uint8_t *buf, uint32_t length) {
        // TODO:
        return Protocol::Status::NOT_FINISHED;
    }
}