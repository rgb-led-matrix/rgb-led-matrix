#include <IO/Protocol/RP2040_UART/Data_Protocol/Query_Request/Query_Request.h>
#include <Exception/String_Exception.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    Query_Request::Query_Request(Node *node, uint8_t magic) {
        status_msg_ = new Status(node, magic);
        magic_ = magic;
        node_ = node;
    }

    Query_Request::~Query_Request() {
        delete status_msg_;
    }

    void Query_Request::run() {
        Status::STATUS current = status_msg_->get_status();
        if (current != Status::STATUS::IDLE_0 && current != Status::STATUS::IDLE_1)
            status = Data_Protocol::Status::ERROR;

        // TODO:
        throw String_Exception("NOT FINISHED");

        status = Data_Protocol::Status::FINISHED;
    }
}