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
    }

    Protocol::~Protocol() {
        node_->free();
    }

    // TODO: Block multi-submit
    //  Calling send too fast can trigger multi-submit
    //      Double buffering, high processing speed, etc.
    void Protocol::send(uint8_t *buf, uint32_t size, uint8_t sizeof_t, uint8_t multiplex, uint8_t columns, uint8_t format) {
        if (buf == nullptr)
            throw Null_Pointer("Buffer");
        
        if (size == 0)
            throw Illegal("Size");

        if (get_protocol_status() == Status::NOT_FINISHED)
            throw Illegal("Protocol still busy");

        buf_ = buf;
        size_ = size;
        sizeof_t_ = sizeof_t;
        multiplex_ = multiplex_;
        columns_ = columns;
        format_ = format;
    }

    Protocol::Status Protocol::get_protocol_status() {
        return internal_state_machine();
    }
}