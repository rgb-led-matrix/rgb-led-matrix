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

    RP2040_UART::RP2040_UART(Node *node, Protocol_Role role, uint8_t magic) : Protocol(node, role) {
        if (role == Protocol_Role::Data) {
            data_ = new Data(node, magic);
            query_ = new Query(node, magic);
        }
        else
            control_ = new Control(node, magic);
    }

    RP2040_UART::~RP2040_UART() {
        if (role_ == Protocol_Role::Data) {
            delete data_;
            delete query_;
        }
        else
            delete control_;
    }

    Protocol::Status RP2040_UART::internal_state_machine(bool clear_errors) {
        Data_Protocol::Status result = data_->send_data(buf_, size_, sizeof_t_, multiplex_, columns_, format_);

        // Block automatic restart!
        if (result == Protocol::Status::NOT_FINISHED)
            buf_ = nullptr;

        if (clear_errors && result == Data_Protocol::Status::ERROR) {
            data_->clear_errors();
            result = data_->send_data(buf_, size_, sizeof_t_, multiplex_, columns_, format_);
        }

        return result;
    }

    void RP2040_UART::internal_signal(Commands command) {
        control_->signal(command);
    }

    float RP2040_UART::get_temperature() {
        // TODO: Query temperature
    }
}