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

    void Protocol::send(uint8_t *buf, uint32_t size, uint8_t scan) {
        lock_.lock();

        if (buf == nullptr)
            throw Null_Pointer("Buffer");
        
        if (size == 0)
            throw Illegal("Size");

        buf_ = buf;
        size_ = size;
        scan_ = scan;
        state_ = 0;
        counter_ = 0;
        status_ = Status::NOT_FINISHED;
    }

    Protocol::Status Protocol::get_protocol_status() {
        return status_;
    }


    // Required transitions:
    //  NOT_FINISHED -> NOT_FINISHED
    //  NOT_FINISHED -> NEXT
    //  NEXT -> NOT_FINISHED
    //  NOT_FINISHED -> FINISHED
    //  FINISHED -> FINISHED (hold till reset)
    // Illegal transisitions:
    //  NEXT -> FINISHED
    //  FINISHED -> NOT_FINISHED
    //  FINISHED -> NEXT
    void Protocol::acknowledge() {
        switch (status_) {
            case Status::NOT_FINISHED:
                status_ = internal_state_machine();
                if (status_ == Status::FINISHED)
                    lock_.unlock();
                break;
            case Status::NEXT:
                status_ = internal_state_machine();
                if (status_ != Status::NOT_FINISHED)
                    throw Illegal("State Machine");
                break;
            case Status::FINISHED:
                break;
            default:
                throw Unknown_Type("Status");
                break;
        }
    }
}