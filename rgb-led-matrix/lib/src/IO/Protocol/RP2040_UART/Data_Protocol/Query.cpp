#include <IO/Protocol/RP2040_UART/Data_Protocol/Query.h>
#include <Exception/Illegal.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    // Do not use this!
    Query::Query() {
        throw Illegal("Query");
    }

    Query::Query(Node *node, uint8_t magic) {
        runnable_ = new Query_Worker(magic);
        runnable_->status = Data_Protocol::Status::FINISHED;
        runnable_->node = node;
        runnable_->magic = magic;
    }

    Query::~Query() {
        // Figure out better solution?
        while (runnable_->status == Data_Protocol::Status::NOT_FINISHED);
        delete runnable_;
    }
}