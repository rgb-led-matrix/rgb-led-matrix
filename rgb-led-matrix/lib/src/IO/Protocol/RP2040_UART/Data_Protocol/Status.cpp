#include <cstring>
#include "IO/Protocol/RP2040_UART/Data_Protocol/Status.h"
#include "IO/Protocol/RP2040_UART/Data_Protocol/Status_msg.h"
#include "IO/Protocol/RP2040_UART/internal.h"
#include "IO/machine.h"
#include "Exception/Illegal.h"

namespace rgb_matrix::Protocol::RP2040_UART {
    Status *Status::status_ptr_ = nullptr;

    Status::Status() {
        // Do not use this!
    }

    Status::Status(Node *node, uint8_t magic) {
        // TODO: Find way to avoid so many threads? (One per protocol instance and each lasts forever!)
        //  We could switch to half duplex and inline the status processing into the data and query logic
        node_ = node;
        shutdown_ = false;
        status_ = STATUS::IDLE_0;
        magic_ = magic;
        thread_ = new std::thread(Status::worker, this);
    }

    Status::~Status() {
        shutdown_ = true;
        thread_->join();
        delete thread_;
    }

    Status *Status::get_status(Node *node, uint8_t magic) {
        if (status_ptr_ == nullptr) {
            status_ptr_ = new Status(node, magic);
        }

        return status_ptr_;
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
        Status_msg m;
        uint8_t buffer[2 * sizeof(Status_msg)];
        uint32_t count = 0;

        while (!obj->shutdown_) {
            uint32_t request = sizeof(buffer) - count;
            uint32_t len = request;

            while (len)
                obj->node_->read(&buffer[count + request - len], &len, 1000);

            // Note this will block up in complexity if add more message types
            memcpy(&m, &buffer[count], sizeof(Status_msg));

            if (!m.valid(obj->magic_))
                ++count;
            else {
                count += sizeof(Status_msg);
                obj->status_ = obj->translate_id(m.status);
            }

            if (count >= sizeof(Status_msg)) {
                count -= sizeof(Status_msg);
                memcpy(&buffer[count], &buffer[count + sizeof(Status_msg)], sizeof(Status_msg));
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
}