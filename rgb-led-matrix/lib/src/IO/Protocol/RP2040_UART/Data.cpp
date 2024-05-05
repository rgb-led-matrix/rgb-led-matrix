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

        status_ = new Status(node);
        runnable_ = new Worker();
        runnable_->status = Protocol::Status::FINISHED;
    }

    Data::~Data() {
        delete status_;

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

    void Data::Worker::run() {
        // TODO:

        status = Protocol::Status::FINISHED;
    }
}