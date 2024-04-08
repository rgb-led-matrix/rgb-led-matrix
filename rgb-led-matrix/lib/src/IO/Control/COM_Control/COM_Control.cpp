#include <IO/Control/COM_Control/COM_Control.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    // Do not use this!
    COM_Control::COM_Control() {
        throw Illegal("COM_Control");
    }

    COM_Control::COM_Control(Node *node) : Control(node) {
        throw String_Exception("Not finished");
        // TODO:
    }

    void COM_Control::signal(Commands command) {
        // TODO:
        switch (command) {
            case Commands::Trigger:
            case Commands::Reset:
            default:
                throw Unknown_Type("Commands");
                break;
        }
    }
}