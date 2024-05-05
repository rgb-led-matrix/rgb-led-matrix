#include <IO/Protocol/RP2040_UART/Status.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    Status::Status() {
        throw Illegal("Status");
    }

    Status::Status(Node *node) {
        throw String_Exception("Not finished");

        // TODO: Create thread for updating status/state
        //  TODO: Find way to avoid so many threads?
    }

    bool Status::get_status(STATUS current, STATUS expected) {
        // TODO:
        
        return false;
    }
}