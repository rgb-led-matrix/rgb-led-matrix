#include <IO/Protocol/RP2040_UART/Data_Protocol/Data.h>
#include <IO/Protocol/RP2040_UART/internal.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>
#include <IO/machine.h>
#include <Panel/RGB/RGB.h>
#include <Panel/RGB/RGB48.h>
#include <Panel/RGB/RGB_555.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    // Do not use this!
    Data::Data() {
        throw Illegal("Data");
    }

    Data::Data(Node *node, uint8_t magic) {
        runnable_ = new Data_Command(node, magic);
        runnable_->status = Data_Protocol::Status::FINISHED;
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
}