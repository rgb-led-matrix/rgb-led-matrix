#include <IO/Protocol/RP2040_UART/Data_Protocol/RP2040_UART_Data.h>
#include <IO/Protocol/RP2040_UART/Data_Protocol/Data.h>
#include <IO/Protocol/RP2040_UART/Data_Protocol/Query.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    RP2040_UART_Data::RP2040_UART_Data() {
        // Do not use this!
    }

    RP2040_UART_Data::RP2040_UART_Data(Node *node, uint8_t id, uint8_t magic) : Data_Protocol(node, id) {
        data_ = new Data(node, magic);
        query_ = new Query(node, magic);
    }

    RP2040_UART_Data::~RP2040_UART_Data() {
        delete data_;
        delete query_;
    }

    // TODO: Refactor this
    Data_Protocol::Status RP2040_UART_Data::internal_state_machine(bool clear_errors) {
        // Claim/release is already done
        Data_Protocol::Status result = data_->send_data(buf_, size_, sizeof_t_, multiplex_, columns_, format_);

        // Block automatic restart!
        if (result == Data_Protocol::Status::NOT_FINISHED) {
            buf_ = nullptr;
        }

        if (clear_errors && result == Data_Protocol::Status::ERROR) {
            data_->clear_errors();
            result = data_->send_data(buf_, size_, sizeof_t_, multiplex_, columns_, format_);
        }

        return result;
    }

    float RP2040_UART_Data::get_temperature(Control_Protocol *protocol) {
        claim();
        // TODO: Query temperature
        //  API still work in progress here
        release();
        return 0.0;
    }
}