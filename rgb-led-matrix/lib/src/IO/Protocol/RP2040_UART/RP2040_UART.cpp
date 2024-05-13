#include <IO/Protocol/RP2040_UART/RP2040_UART.h>
#include <IO/Protocol/RP2040_UART/Data.h>
#include <IO/Protocol/RP2040_UART/Query.h>
#include <IO/Protocol/RP2040_UART/Control.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    RP2040_UART::RP2040_UART() {
        throw Illegal("RP2040_UART");
    }

    RP2040_UART::RP2040_UART(Node *data, Node *control, uint8_t magic) : Protocol(data, control) {
        data_ = new Data(data, magic);
        query_ = new Query(data, magic);
        control_ = new Control(control, magic);
    }

    RP2040_UART::~RP2040_UART() {
        delete data_;
        delete query_;
        delete control_;
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

    void RP2040_UART::signal(Commands command) {
        control_->signal(command);
    }
}