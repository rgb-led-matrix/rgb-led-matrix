#include <IO/Protocol/RP2040_UART/RP2040_UART.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    RP2040_UART::RP2040_UART() {
        throw Illegal("RP2040_UART");
    }

    RP2040_UART::RP2040_UART(Node *node) : Protocol(node) {
        throw String_Exception("Not finished");
        prev_ = 'y';

        // TODO: Create thread for updating status/state
        //  TODO: Find way to avoid so many threads?
    }

    Protocol::Status RP2040_UART::internal_state_machine() {
        Protocol::Status result = Protocol::Status::NOT_FINISHED;
        uint8_t str[10];
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
        lock_.lock();
        switch (state_) {
            // TODO: Replace this with new protocol
            case 0:
                if (counter_ >= scan_)  // Done flag
                    state_ = 1;
                else {
                    state_ = 2;
                    size = size_ / scan_;
                    node_->write((char *) buf_ + (size * counter_), size);
                    ++counter_;
                }
                break;
            case 1:
                result = Protocol::Status::FINISHED;
                break;
            case 2:
                ack = false;
                size = node_->read((char **) &str, sizeof(str), (uint32_t) 2);
                for (uint32_t i = 0; i < size; i++) {
                    if ((str[i] != prev_) && (str[i] == 'y' || str[i] == 'n'))
                        prev_ = str[i];

                    if (prev_ == 'n' && str[i] == 'y') {
                        ack |= true;
                        prev_ = 'y';
                    }
                }

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
        lock_.unlock();

        return result;
    }

    void RP2040_UART::write(frame_head *data) {
        // TODO
    }

    void RP2040_UART::write(frame_tail *data) {
        // TODO
    }

    void RP2040_UART::read(frame_head *data) {
        // TODO
    }

    void RP2040_UART::read(frame_tail *data) {
        // TODO
    }
}