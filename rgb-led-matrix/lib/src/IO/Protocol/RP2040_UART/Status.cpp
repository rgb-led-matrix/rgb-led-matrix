#include <cstring>
#include <IO/Protocol/RP2040_UART/Status.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    Status::Status() {
        throw Illegal("Status");
    }

    Status::Status(Node *node) {
        throw String_Exception("Not finished");

        // Find way to avoid so many threads?
        node_ = node;
        shutdown_ = false;
        status_ = STATUS::READY;
        thread_ = new std::thread(Status::worker, this);
    }

    Status::~Status() {
        shutdown_ = true;
        thread_->join();
        delete thread_;
    }

    bool Status::get_status(STATUS current, STATUS expected) {
        STATUS result = get_status();

        if (result == expected)
            return true;

        if (result != current)
            throw Illegal("Unexpected status");
        
        return false;
    }

    Status::STATUS Status::get_status() {
        return status_;
    }

    // Stupid byte stream with it's simple parser
    //  This will never ever be aligned!
    //  Can't afford a real protocol using packets...
    void Status::worker(Status *obj) {
        msg m;
        uint8_t buffer[2 * sizeof(msg)];
        uint32_t count = 0;

        while (!obj->shutdown_) {
            uint32_t request = sizeof(buffer) - count;
            uint32_t len = request;

            while (len)
                obj->node_->read(&buffer[count + request - len], &len, 1000);


            // Note this will block up in complexity if add more message types
            memcpy(&m, &buffer[count], sizeof(msg));

            if (!m.valid())
                ++count;
            else {
                count += sizeof(msg);
                obj->status_ = obj->translate_id(m.cmd);
            }

            if (count >= sizeof(msg)) {
                count -= sizeof(msg);
                memcpy(&buffer[count], &buffer[count + sizeof(msg)], sizeof(msg));
            }
        }
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

    bool Status::msg::valid() {
        // TODO:
        return false;
    }
}