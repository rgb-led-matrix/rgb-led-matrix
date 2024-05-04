#include <IO/Protocol/Protocol.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    ThreadPool<volatile bool *, void *> *Protocol::pool_ = nullptr;

    // Do not use this!
    Protocol::Protocol() {
        Illegal("Protocol");
    }

    Protocol::Protocol(Node *node, ThreadPool<volatile bool *, void *> *pool) {
        if (node == nullptr)
            throw Null_Pointer("Node");
        
        node_ = node;
        custom_pool_ = pool;
    }

    void Protocol::send(uint8_t *buf, uint32_t size, uint8_t scan) {
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
    //  NEXT -> NEXT
    //  FINISHED -> NOT_FINISHED
    //  FINISHED -> NEXT
    void Protocol::acknowledge() {
        switch (status_) {
            case Status::NOT_FINISHED:
                status_ = internal_state_machine();
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

    ThreadPool<volatile bool *, void *> *Protocol::get_threadpool() {
        if (custom_pool_ == nullptr) {
            return Protocol::get_static_threadpool();
        }

        return custom_pool_;
    }

    ThreadPool<volatile bool *, void *> *Protocol::get_static_threadpool() {
        if (pool_ == nullptr) {
            pool_ = new ThreadPool<volatile bool *, void *>();
            pool_->start();
        }

        return pool_;
    }
}