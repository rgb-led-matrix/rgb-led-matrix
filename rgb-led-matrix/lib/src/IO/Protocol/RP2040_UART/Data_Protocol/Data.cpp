#include <IO/Protocol/RP2040_UART/Data_Protocol/Data.h>
#include <IO/Protocol/RP2040_UART/internal.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>
#include <IO/machine.h>
#include <Panel/RGB/RGB.h>
#include <Panel/RGB/RGB24.h>
#include <Panel/RGB/RGB48.h>
#include <Panel/RGB/RGB_555.h>
#include <Panel/RGB/RGB_222.h>

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

    Data_Protocol::Status Data::send_data(void *buf, uint32_t length, uint8_t sizeof_t, uint8_t multiplex, uint8_t columns, Data_Format_ID format) {
        uint32_t i;

        // TODO: Fix this
        RGB48 *rgb_long = (RGB48 *) buf;
        uint16_t *rgb_short = (uint16_t *) buf;

        // Start if ready (No error handling required here)
        if (runnable_->status == Data_Protocol::Status::FINISHED && buf != nullptr) {
            runnable_->status = Data_Protocol::Status::NOT_FINISHED;

            // TODO: Fix this
            switch (format) {
                case Data_Format_ID::RGB48_ID:
                    for (i = 0; i < length / 6; i++) {
                        rgb_long[i].red = htons(rgb_long[i].red);
                        rgb_long[i].green = htons(rgb_long[i].green);
                        rgb_long[i].blue = htons(rgb_long[i].blue);
                    }
                    break;
                case Data_Format_ID::RGB_555_ID:
                    for (i = 0; i < length / 2; i++)
                        rgb_short[i] = htons(rgb_short[i]);
                    break;
                case Data_Format_ID::RGB24_ID:
                case Data_Format_ID::RGB_222_ID:
                    break;
                default:
                    throw Unknown_Type("Data_Format_ID");
                    break;
            }

            runnable_->buffer = (uint8_t *) buf;
            runnable_->length = length;
            runnable_->sizeof_t = sizeof_t;
            runnable_->multiplex = multiplex;
            runnable_->columns = columns;
            runnable_->format = RGB::translate_id(format);
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

    bool Data::Worker::wait(Status::STATUS current, Status::STATUS expected, uint32_t timeout_us) {
        while (!status_msg_->get_status(current, expected)) {
            // TODO: Check for timeout
        }

        return true;
    }

    void Data::Worker::run() {
        uint32_t checksum = 0xFFFFFFFF;
        uint32_t header = htonl(internal::generate_header(magic));
        uint8_t cmd[2] = {'d', 'd'};
        uint16_t len = htons(length);

        Status::STATUS current = status_msg_->get_status();
        if (current != Status::STATUS::IDLE_0 && current != Status::STATUS::IDLE_1)
            status = Data_Protocol::Status::ERROR;

        // PREAMBLE_CMD_LEN_T_MULTIPLEX_COLUMNS
        checksum = internal::checksum_chunk(checksum, header, 32);
        node->write(header);
        checksum = internal::checksum_chunk(checksum, len, 16);
        node->write(len);
        checksum = internal::checksum_chunk(checksum, cmd[0], 8);
        node->write(cmd[0]);
        checksum = internal::checksum_chunk(checksum, cmd[1], 8);
        node->write(cmd[1]);
        checksum = internal::checksum_chunk(checksum, sizeof_t, 8);
        node->write(sizeof_t);
        checksum = internal::checksum_chunk(checksum, multiplex, 8);
        node->write(multiplex);
        checksum = internal::checksum_chunk(checksum, columns, 8);
        node->write(columns);
        checksum = internal::checksum_chunk(checksum, format, 8);
        node->write(format);
        
        if (!wait(current, Status::STATUS::ACTIVE_0, 100)) {
            status = Data_Protocol::Status::ERROR;
            return;
        }

        // PAYLOAD
        for (uint32_t i = 0; i < length; i++)
            checksum = internal::checksum_chunk(checksum, buffer[i], 8);
        node->write(buffer, length);

        if (!wait(status_msg_->get_status(), Status::STATUS::ACTIVE_1, 100)) {
            status = Data_Protocol::Status::ERROR;
            return;
        }

        // TODO:
        throw String_Exception("NOT FINISHED");

        status = Data_Protocol::Status::FINISHED;
    }
}