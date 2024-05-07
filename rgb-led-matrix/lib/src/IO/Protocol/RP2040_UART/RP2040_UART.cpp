#include <IO/Protocol/RP2040_UART/RP2040_UART.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    RP2040_UART::RP2040_UART() {
        throw Illegal("RP2040_UART");
    }

    RP2040_UART::RP2040_UART(Node *node) : Protocol(node) {
        data_ = new Data(node);
    }

    Protocol::Status RP2040_UART::internal_state_machine(bool clear_errors) {
        if (clear_errors)
            data_->clear_errors();
            
        return data_->send_data(buf_, size_, sizeof_t_, multiplex_, columns_, format_);
    }
}