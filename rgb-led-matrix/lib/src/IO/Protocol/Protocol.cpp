#include <IO/Protocol/Protocol.h>
#include <Exception/Null_Pointer.h>

namespace rgb_matrix {
    // Do not use this!
    Protocol::Protocol() {
        // Do nothing
    }

    Protocol::Protocol(Node *node) {
        if (node == nullptr)
            throw Null_Pointer("Node");
        
        node_ = node;
    }
}