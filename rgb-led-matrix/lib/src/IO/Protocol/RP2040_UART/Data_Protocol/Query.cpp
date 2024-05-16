#include <IO/Protocol/RP2040_UART/Data_Protocol/Query.h>
#include <Exception/Illegal.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    // Do not use this!
    Query::Query() {
        throw Illegal("Status");
    }

    Query::Query(Node *node, uint8_t magic) {
        runnable_ = new Worker(magic);
        runnable_->status = Data_Protocol::Status::FINISHED;
        runnable_->node = node;
        runnable_->magic = magic;
    }

    Query::~Query() {
        // Figure out better solution?
        while (runnable_->status == Data_Protocol::Status::NOT_FINISHED);
        delete runnable_;
    }

    Query::Worker::Worker(uint8_t magic) {
        status_msg_ = new Status(node, magic);
    }

    Query::Worker::~Worker() {
        delete status_msg_;
    }

    void Query::Worker::run() {
        Status::STATUS current = status_msg_->get_status();
        if (current != Status::STATUS::IDLE_0 && current != Status::STATUS::IDLE_1)
            status = Data_Protocol::Status::ERROR;

        // TODO:
        throw String_Exception("NOT FINISHED");

        status = Data_Protocol::Status::FINISHED;
    }
}