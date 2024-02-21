#include <IO/Protocol/RP2040_UART/RP2040_UART.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    RP2040_UART::RP2040_UART() {
        throw Illegal("RP2040_UART");
    }

    RP2040_UART::RP2040_UART(Node *node) : Protocol(node) {
        throw String_Exception("Not finished");
    }

    Protocol::Status RP2040_UART::internal_state_machine() {
        Protocol::Status result = Protocol::Status::NOT_FINISHED;
        uint32_t size;
        bool ack;

        // Required transitions:
        //  NOT_FINISHED -> NOT_FINISHED (if not state 0 done flag and/or if not state 2 done flag)
        //  NOT_FINISHED -> NEXT (If state 2 done flag)
        //  NEXT -> NOT_FINISHED
        //  NOT_FINISHED -> FINISHED (if state 0 done flag)
        //  FINISHED -> FINISHED (hold till reset)
        // Illegal transisitions:
        //  NEXT -> FINISHED
        //  NEXT -> NEXT
        //  FINISHED -> NOT_FINISHED
        //  FINISHED -> NEXT
        switch (state_) {
            case 0:
                if (counter_ >= scan_)  // Done flag
                    state_ = 1;
                else
                    state_ = 2;

                size = size_ / scan_;
                node_->write((char *) buf_ + (size * counter_), size);
                ++counter_;
                break;
            case 1:
                result = Protocol::Status::FINISHED;
                break;
            case 2:
                ack = false;
                // TODO: Wait for ack

                if (ack) {              // Done flag
                    state_ = 0;
                    result = Protocol::Status::NEXT;
                }
                break;
            default:
                state_ = 0;
                throw Illegal("State");
                break;
        }

        return result;
    }
}