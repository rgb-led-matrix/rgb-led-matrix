#include <IO/Protocol/RP2040_UART/RP2040_UART.h>
#include <Exception/String_Exception.h>

namespace rgb_matrix {
    RP2040_UART::RP2040_UART(Node *node) : Protocol(node) {
        throw String_Exception("Not finsihed");
    }

    void RP2040_UART::send(uint8_t *buf, uint32_t size) {
        /*uint32_t size = sizeof(T) * object->cfg_->get_cols() * object->cfg_->get_rows();
        char *start = (char *) "s";
        char *idle = (char *) "i";

        while (object->shutdown_) {
            if (object->start_) {
                // Send active tocken to get buss out of idle
                object->cfg_->get_node()->write(start, 1);

                // Wait for bus to become active (timout after 100uS and drop frame)
                for (int i = 0; i < 10; i++) {
                    // If it times out then the bus is idle, send frame
                    if (object->cfg_->get_node()->read(&idle, 1, 10) == 0) {
                        // TODO: Send checksum of data using CRC-32/MPEG-2

                        object->cfg_->get_node()->write((char *) object->buffer_, size);

                        // Check for idle loop, recover if still active
                        while (object->cfg_->get_node()->read(&idle, 1, 100) == 0)
                            object->cfg_->get_node()->write(idle, 1);
                    
                        break;
                    }
                }

                object->start_ = false;
            }

            // Sink idle tokens
            object->cfg_->get_node()->read(&idle, 1, 10);
        }*/
    }

    Protocol::Status RP2040_UART::get_protocol_status() {
        // TODO
        return Protocol::Status::NOT_FINISHED;
    }

    void RP2040_UART::acknowledge(Protocol::Status status) {
        // TODO
    }
}