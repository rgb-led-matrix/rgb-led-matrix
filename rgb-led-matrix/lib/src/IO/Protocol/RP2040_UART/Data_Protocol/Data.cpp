#include <IO/Protocol/RP2040_UART/Data_Protocol/Data.h>
#include <Exception/Illegal.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    // Do not use this!
    Data::Data() {
        throw Illegal("Status");
    }

    Data::Data(Node *node, uint8_t magic) {
        runnable_ = new Worker(magic);
        runnable_->status = Data_Protocol::Status::FINISHED;
        runnable_->node = node;
        runnable_->magic = magic;
    }

    Data::~Data() {
        // Figure out better solution?
        while (runnable_->status == Data_Protocol::Status::NOT_FINISHED);
        delete runnable_;
    }

    Data_Protocol::Status Data::send_data(uint8_t *buf, uint32_t length, uint8_t sizeof_t, uint8_t multiplex, uint8_t columns, uint8_t format) {
        // Start if ready (No error handling required here)
        if (runnable_->status == Data_Protocol::Status::FINISHED && buf != nullptr) {
            runnable_->status = Data_Protocol::Status::NOT_FINISHED;
            runnable_->buffer = buf;
            runnable_->length = length;
            runnable_->sizeof_t = sizeof_t;
            runnable_->multiplex = multiplex;
            runnable_->columns = columns;
            runnable_->format = format;
            ThreadPool::get_threadpool(ThreadPool::Pool_ID::IO)->submit(runnable_);
        }

        // Always report status
        return runnable_->status;
    }

    void Data::clear_errors() {
        if (runnable_->status == Data_Protocol::Status::ERROR)
            runnable_->status = Data_Protocol::Status::FINISHED;
    }

    Data::Worker::Worker(uint8_t magic) {
        status_msg_ = new Status(node, magic);
    }

    Data::Worker::~Worker() {
        delete status_msg_;
    }

    void Data::Worker::run() {
        Status::STATUS current = status_msg_->get_status();
        if (current != Status::STATUS::IDLE_0 && current != Status::STATUS::IDLE_1)
            status = Data_Protocol::Status::ERROR;

        // TODO:
        throw String_Exception("NOT FINISHED");

        status = Data_Protocol::Status::FINISHED;
    }
}