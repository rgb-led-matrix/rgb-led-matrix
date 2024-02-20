#include <IO/Protocol/RP2040_UART/RP2040_UART.h>
#include <Exception/Illegal.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    // Do not use this!
    RP2040_UART::RP2040_UART() {
        throw Illegal("RP2040_UART");
    }

    RP2040_UART::RP2040_UART(Node *node) : Protocol(node) {
        throw String_Exception("Not finished");
    }

    Protocol::Status RP2040_UART::internal_state_machine() {
        // TODO:
        return Protocol::Status::NOT_FINISHED;
    }
}