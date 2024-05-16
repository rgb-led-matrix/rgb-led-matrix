#include <IO/Protocol/RP2040_UART/Data_Protocol/RP2040_UART_Data.h>
#include <IO/Protocol/RP2040_UART/Data_Protocol/Data.h>
#include <IO/Protocol/RP2040_UART/Data_Protocol/Query.h>
#include <IO/Protocol/RP2040_UART/Control_Protocol/Control.h>
#include <Exception/Illegal.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    // Do not use this!
    RP2040_UART_Data::RP2040_UART_Data() {
        throw Illegal("RP2040_UART Data");
    }

    RP2040_UART_Data::RP2040_UART_Data(Node *node, uint8_t magic) : Data_Protocol(node) {
        data_ = new Data(node, magic);
        query_ = new Query(node, magic);
    }

    RP2040_UART_Data::~RP2040_UART_Data() {
        delete data_;
        delete query_;
    }

    Data_Protocol::Status RP2040_UART_Data::internal_state_machine(bool clear_errors) {
        Data_Protocol::Status result = data_->send_data(buf_, size_, sizeof_t_, multiplex_, columns_, format_);

        // Block automatic restart!
        if (result == Data_Protocol::Status::NOT_FINISHED)
            buf_ = nullptr;

        if (clear_errors && result == Data_Protocol::Status::ERROR) {
            data_->clear_errors();
            result = data_->send_data(buf_, size_, sizeof_t_, multiplex_, columns_, format_);
        }

        return result;
    }

    float RP2040_UART_Data::get_temperature(Control_Protocol *protocol) {
        // TODO: Query temperature
        return 0.0;
    }
}