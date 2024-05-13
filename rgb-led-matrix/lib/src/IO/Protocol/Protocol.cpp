#include <IO/Protocol/Protocol.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    // Do not use this!
    Protocol::Protocol() {
        Illegal("Protocol");
    }

    Protocol::Protocol(Node *node) {
        if (node == nullptr)
            throw Null_Pointer("Node");
        
        if (!node->claim())
            throw Illegal("Attempt to node in use");
        
        node_ = node;
        buf_ = nullptr;
        claim_ = false;
    }

    Protocol::~Protocol() {
        node_->free();
    }

    void Protocol::send(uint8_t *buf, uint32_t size, uint8_t sizeof_t, uint8_t multiplex, uint8_t columns, uint8_t format) {
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

    Protocol::Status Protocol::get_protocol_status() {
        return get_protocol_status(false);
    }

    Protocol::Status Protocol::get_protocol_status(bool clear_errors) {
        Protocol::Status result = internal_state_machine(clear_errors);

        // It has finished in some way, so allow new submission
        if (result != Protocol::Status::NOT_FINISHED && !clear_errors)
            release();

        return result;
    }

    void Protocol::claim() {
        lock_.lock();
        claim_ = true;
        lock_.unlock();
    }

    void Protocol::release() {
        lock_.lock();
        claim_ = false;
        lock_.unlock();
    }
}