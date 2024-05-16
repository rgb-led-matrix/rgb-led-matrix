#include <IO/Protocol/Control_Protocol.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    Control_Protocol::Control_Protocol() {
        Illegal("Control Protocol");
    }

    Control_Protocol::Control_Protocol(Node *node) {
        if (node == nullptr)
            throw Null_Pointer("Node");
        
        if (!node->claim())
            throw Illegal("Attempt to node in use");
        
        node_ = node;
        claim_ = false;
    }

    Control_Protocol::~Control_Protocol() {
        node_->free();
    }

    void Control_Protocol::signal(Commands command) {
        internal_signal(command);
    }

    void Control_Protocol::claim() {
        lock_.lock();
        claim_ = true;
        lock_.unlock();
    }

    void Control_Protocol::release() {
        lock_.lock();
        claim_ = false;
        lock_.unlock();
    }
}