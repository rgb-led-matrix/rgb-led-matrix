#include <IO/Protocol/RP2040_UART/Control_Protocol/RP2040_UART_Control.h>
#include <IO/Protocol/RP2040_UART/Control_Protocol/Control.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    RP2040_UART_Control::RP2040_UART_Control() {
        // Do not use this! 
    }

    RP2040_UART_Control::RP2040_UART_Control(Node *node, uint8_t magic) : Control_Protocol(node) {
        control_ = new Control(node, magic);
    }

    RP2040_UART_Control::~RP2040_UART_Control() {
        delete control_;
    }
    void RP2040_UART_Control::internal_signal(Commands command) {
        control_->signal(command);
    }
}