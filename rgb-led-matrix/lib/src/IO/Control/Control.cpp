#include <IO/Control/Control.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    Control::Control() {
        Illegal("Control");
    }

    Control::Control(Node *node) {
        if (node == nullptr)
            throw Null_Pointer("Node");
        
        node_ = node;
    }
}