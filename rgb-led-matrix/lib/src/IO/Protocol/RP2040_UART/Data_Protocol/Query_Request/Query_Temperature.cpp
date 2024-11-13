#include "IO/Protocol/RP2040_UART/Data_Protocol/Query_Request/Query_Temperature.h"
#include "Exception/String_Exception.h"

namespace rgb_matrix::Protocol::RP2040_UART {
    Query_Temperature::Query_Temperature(Node *node, uint8_t magic) {
        status_msg_ = Status::get_status(node, magic);
        magic_ = magic;
        node_ = node;
    }

    Query_Temperature::~Query_Temperature() {
        delete status_msg_;
    }

    void Query_Temperature::run() {
        Status::STATUS current = status_msg_->get_status();
        if (current != Status::STATUS::IDLE_0 && current != Status::STATUS::IDLE_1)
            status = Data_Protocol::Status::ERROR;

        // TODO:
        throw String_Exception("NOT FINISHED");

        status = Data_Protocol::Status::FINISHED;
    }
}