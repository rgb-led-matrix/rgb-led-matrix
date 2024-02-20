#include <IO/Protocol/RP2040_UART/RP2040_UART.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    RP2040_UART::RP2040_UART() {
        throw Illegal("RP2040_UART");
    }

    RP2040_UART::RP2040_UART(Node *node) : Protocol(node) {
        throw String_Exception("Not finished");

        state_ = 0;
        counter_ = 0;
    }

    Protocol::Status RP2040_UART::internal_state_machine() {
        Protocol::Status result = Protocol::Status::NOT_FINISHED;

        switch (state_) {
            case 0:
                // TODO:
                break;
            default:
                throw Illegal("State");
                break;
        }

        return result;
    }
}