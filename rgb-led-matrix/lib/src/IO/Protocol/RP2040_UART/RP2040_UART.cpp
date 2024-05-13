#include <IO/Protocol/RP2040_UART/RP2040_UART.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    RP2040_UART::RP2040_UART() {
        throw Illegal("RP2040_UART");
    }

    RP2040_UART::RP2040_UART(Node *node, uint8_t magic) : Protocol(node) {
        data_ = new Data(node, magic);
        query_ = new Query(node, magic);
    }

    RP2040_UART::~RP2040_UART() {
        delete data_;
        delete query_;
    }

    Protocol::Status RP2040_UART::internal_state_machine(bool clear_errors) {
        Protocol::Status result = data_->send_data(buf_, size_, sizeof_t_, multiplex_, columns_, format_);

        // Block automatic restart!
        if (result == Protocol::Status::NOT_FINISHED)
            buf_ = nullptr;

        if (clear_errors && result == Protocol::Status::ERROR) {
            data_->clear_errors();
            result = data_->send_data(buf_, size_, sizeof_t_, multiplex_, columns_, format_);
        }

        return result;
    }
}