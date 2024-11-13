#ifndef RP2040_UART_DATA_PROTOCOL_DATA_COMMAND_H
#define RP2040_UART_DATA_PROTOCOL_DATA_COMMAND_H

#include <stdint.h>
#include "ThreadPool/ThreadPool.h"
#include "IO/Protocol/Data_Protocol.h"
#include "IO/Protocol/RP2040_UART/Data_Protocol/Status.h"

namespace rgb_matrix::Protocol::RP2040_UART {
    class Data_Command : public Runnable {
        public:
            Data_Command(Node *node, uint8_t magic, bool checksum = true);
            ~Data_Command();

            void run();

            uint8_t *buffer;
            uint32_t length;
            uint8_t sizeof_t;
            uint8_t multiplex;
            uint8_t columns;
            uint8_t format;
            rgb_matrix::Data_Protocol::Status status;
        
        private:
            bool wait(Status::STATUS current, Status::STATUS expected, uint32_t timeout_us);

            rgb_matrix::Node *node_;
            Status *status_msg_;
            uint8_t magic_;
            bool checksum_;
    };

}
#endif
