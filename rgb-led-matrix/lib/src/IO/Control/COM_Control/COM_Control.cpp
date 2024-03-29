#include <IO/Control/COM_Control/COM_Control.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    COM_Control::COM_Control() {
        throw Illegal("COM_Control");
    }

    COM_Control::COM_Control(Node *node) : Control(node) {
        throw String_Exception("Not finished");
        // TODO:
    }

    void COM_Control::signal() {
        // TODO:
    }
}