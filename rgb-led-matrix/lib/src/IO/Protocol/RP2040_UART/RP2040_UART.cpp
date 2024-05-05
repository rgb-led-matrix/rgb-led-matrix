#include <IO/Protocol/RP2040_UART/RP2040_UART.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    RP2040_UART::RP2040_UART() {
        throw Illegal("RP2040_UART");
    }

    RP2040_UART::RP2040_UART(Node *node) : Protocol(node) {
        throw String_Exception("Not finished");

        // TODO: Create thread for updating status/state
        //  TODO: Find way to avoid so many threads?
    }

    Protocol::Status RP2040_UART::internal_state_machine() {
        lock_.lock();
        switch (state_) {
            // TODO: Replace this with new protocol
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            default:
                state_ = 0;
                throw Illegal("State");
                break;
        }
        lock_.unlock();
        return status_;
    }
}