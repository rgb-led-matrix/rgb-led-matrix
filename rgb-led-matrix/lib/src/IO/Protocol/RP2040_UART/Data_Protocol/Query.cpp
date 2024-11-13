#include "IO/Protocol/RP2040_UART/Data_Protocol/Query.h"

namespace rgb_matrix::Protocol::RP2040_UART {
    Query::Query() {
        // Do not use this!
    }

    Query::Query(Node *node, uint8_t magic) {
        //runnable_ = new Query_Request(node, magic);
        //runnable_->status = Data_Protocol::Status::FINISHED;
    }

    Query::~Query() {
        // Figure out better solution?
        //while (runnable_->status == Data_Protocol::Status::NOT_FINISHED);
        delete runnable_;
    }
}