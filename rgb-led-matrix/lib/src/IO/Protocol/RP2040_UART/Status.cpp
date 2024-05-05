#include <IO/Protocol/RP2040_UART/Status.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    Status::Status() {
        throw Illegal("Status");
    }

    Status::Status(Node *node) {
        throw String_Exception("Not finished");

        node_ = node;

        // TODO: Create thread for updating status/state
        //  TODO: Find way to avoid so many threads?
    }

    bool Status::get_status(STATUS current, STATUS expected) {
        // TODO:
        
        return false;
    }

    Status::STATUS Status::translate_id(uint32_t id) {
        STATUS result;

        switch (id) {
            case 0:
                result = STATUS::IDLE_0;
                break;
            case 1:
                result = STATUS::IDLE_1;
                break;
            case 2:
                result = STATUS::ACTIVE_0;
                break;
            case 3:
                result = STATUS::ACTIVE_1;
                break;
            case 4:
                result = STATUS::READY;
                break;
            default:
                throw Illegal("Unknown Status ID");
                break;
        }

        return result;
    }
}