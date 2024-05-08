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
    }

    Protocol::~Protocol() {
        node_->free();
    }

    void Protocol::send(uint8_t *buf, uint32_t size, uint8_t sizeof_t, uint8_t multiplex, uint8_t columns, uint8_t format) {
        if (buf == nullptr)
            throw Null_Pointer("Buffer");
        
        if (size == 0)
            throw Illegal("Size");

        lock_.lock();

        // Clear errors and update state of buf_
        get_protocol_status(true);

        // Check status of protocol state
        if (buf_ != nullptr) {
            lock_.unlock();
            throw Illegal("Protocol still busy");
        }

        // Setup protocol for another run
        buf_ = buf;
        size_ = size;
        sizeof_t_ = sizeof_t;
        multiplex_ = multiplex_;
        columns_ = columns;
        format_ = format;
        lock_.unlock();
    }

    Protocol::Status Protocol::get_protocol_status() {
        return get_protocol_status(false);
    }

    Protocol::Status Protocol::get_protocol_status(bool clear_errors) {
        Protocol::Status result = internal_state_machine(clear_errors);

        // It has finished in some way, so allow new submission
        if (result != Protocol::Status::NOT_FINISHED) {
            lock_.lock();
            buf_ = nullptr;
            lock_.unlock();
        }

        return result;
    }
}