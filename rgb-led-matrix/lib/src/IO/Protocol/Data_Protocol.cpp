#include "IO/Protocol/Data_Protocol.h"
#include "Exception/Null_Pointer.h"
#include "Exception/Illegal.h"

namespace rgb_matrix {
    Data_Protocol::Data_Protocol() {
        // Do not use this! 
    }

    Data_Protocol::Data_Protocol(Node *node, uint8_t id) {
        if (node == nullptr)
            throw Null_Pointer("Node");
        
        if (!node->claim())
            throw Illegal("Attempt to node in use");
        
        node_ = node;
        buf_ = nullptr;
        claim_ = false;
        id_ = id;
    }

    Data_Protocol::~Data_Protocol() {
        node_->free();
    }

    void Data_Protocol::send(void *buf, uint32_t size, uint8_t sizeof_t, uint8_t multiplex, uint8_t columns, Data_Format_ID format) {
        if (buf == nullptr)
            throw Null_Pointer("Buffer");
        
        if (size == 0)
            throw Illegal("Size");

        // Clear errors and submit another run
        claim();
        get_protocol_status(true);

        // Setup protocol for another run
        buf_ = buf;
        size_ = size;
        sizeof_t_ = sizeof_t;
        multiplex_ = multiplex_;
        columns_ = columns;
        format_ = format;
    }

    Data_Protocol::Status Data_Protocol::get_protocol_status() {
        return get_protocol_status(false);
    }

    Data_Protocol::Status Data_Protocol::get_protocol_status(bool clear_errors) {
        Status result = internal_state_machine(clear_errors);

        // It has finished in some way, so allow new submission
        if (result != Status::NOT_FINISHED && !clear_errors)
            release();

        return result;
    }

    void Data_Protocol::claim() {
        lock_.lock();
        claim_ = true;
        lock_.unlock();
    }

    void Data_Protocol::release() {
        lock_.lock();
        claim_ = false;
        lock_.unlock();
    }
}