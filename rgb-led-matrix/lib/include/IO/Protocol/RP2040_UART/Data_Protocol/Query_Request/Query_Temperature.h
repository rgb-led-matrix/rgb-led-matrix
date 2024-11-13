#ifndef RP2040_UART_DATA_PROTOCOL_QUERY_TEMPERATURE_H
#define RP2040_UART_DATA_PROTOCOL_QUERY_TEMPERATURE_H

#include <stdint.h>
#include "ThreadPool/ThreadPool.h"
#include "IO/Protocol/Data_Protocol.h"
#include "IO/Protocol/RP2040_UART/Data_Protocol/Status.h"

namespace rgb_matrix::Protocol::RP2040_UART {
    class Query_Temperature : public Runnable {
        public:
            Query_Temperature(Node *node, uint8_t magic);
            ~Query_Temperature();

            void run();

            Data_Protocol::Status status;
        
        private:
            Node *node_;
            Status *status_msg_;
            uint8_t magic_;

    };
}
#endif
