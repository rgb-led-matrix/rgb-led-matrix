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

    Protocol::Status RP2040_UART::internal_state_machine() {
        lock_.lock();
        status_ = data_->send_data(buf_, size_);
        lock_.unlock();
        return status_;
    }
}