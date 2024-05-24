#ifndef RP2040_UART_DATA_PROTOCOL_QUERY_WORKER_H
#define RP2040_UART_DATA_PROTOCOL_QUERY_WORKER_H

#include <stdint.h>
#include <ThreadPool/ThreadPool.h>
#include <IO/Protocol/Data_Protocol.h>
#include <IO/Protocol/RP2040_UART/Data_Protocol/Status.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    class Query_Worker : public Runnable {
        public:
            Query_Worker(uint8_t magic);
            ~Query_Worker();

            void run();

            Node *node;
            Data_Protocol::Status status;
            uint8_t magic;
        
        private:
            Status *status_msg_;

    };
}
#endif
