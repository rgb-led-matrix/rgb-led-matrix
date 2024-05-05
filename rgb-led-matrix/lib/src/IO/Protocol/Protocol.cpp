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
        
        node_ = node;
    }

    void Protocol::send(uint8_t *buf, uint32_t size) {
        if (buf == nullptr)
            throw Null_Pointer("Buffer");
        
        if (size == 0)
            throw Illegal("Size");

        if (get_protocol_status() == Status::NOT_FINISHED)
            throw Illegal("Protocol still busy");

        buf_ = buf;
        size_ = size;
    }

    Protocol::Status Protocol::get_protocol_status() {
        return internal_state_machine();
    }
}