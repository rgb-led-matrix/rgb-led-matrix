#include <IO/Protocol/RP2040_UART/Data_Protocol/Query_Worker.h>
#include <Exception/String_Exception.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    Query_Worker::Query_Worker(uint8_t magic) {
        status_msg_ = new Status(node, magic);
    }

    Query_Worker::~Query_Worker() {
        delete status_msg_;
    }

    void Query_Worker::run() {
        Status::STATUS current = status_msg_->get_status();
        if (current != Status::STATUS::IDLE_0 && current != Status::STATUS::IDLE_1)
            status = Data_Protocol::Status::ERROR;

        // TODO:
        throw String_Exception("NOT FINISHED");

        status = Data_Protocol::Status::FINISHED;
    }
}