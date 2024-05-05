#include <IO/Protocol/RP2040_UART/Data.h>
#include <IO/Protocol/Protocol.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    Data::Data() {
        throw Illegal("Status");
    }

    Data::Data(Node *node) {
        throw String_Exception("Not finished");

        runnable_ = new Worker();
        runnable_->status = Protocol::Status::FINISHED;
        runnable_->node = node;
    }

    Data::~Data() {
        // TODO: Figure out better solution?
        while (runnable_->status == Protocol::Status::NOT_FINISHED);
        delete runnable_;
    }

    Protocol::Status Data::send_data(uint8_t *buf, uint32_t length) {
        if (runnable_->status == Protocol::Status::FINISHED) {
            ThreadPool::get_threadpool()->submit(runnable_);
            runnable_->status = Protocol::Status::NOT_FINISHED;
        }

        return runnable_->status;
    }

    Data::Worker::Worker() {
        status_msg_ = new Status(node);
    }

    Data::Worker::~Worker() {
        delete status_msg_;
    }

    void Data::Worker::run() {
        if (status == Protocol::Status::NOT_FINISHED) {
            if (status_msg_->get_status() != Status::STATUS::READY)
                status = Protocol::Status::ERROR;

            // TODO:

            status = Protocol::Status::FINISHED;
        }
    }
}