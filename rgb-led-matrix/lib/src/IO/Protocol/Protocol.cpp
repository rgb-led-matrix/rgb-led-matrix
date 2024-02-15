#include <IO/Protocol/Protocol.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>

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
}