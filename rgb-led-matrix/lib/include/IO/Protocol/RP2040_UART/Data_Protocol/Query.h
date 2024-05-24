#ifndef RP2040_UART_DATA_PROTOCOL_QUERY_H
#define RP2040_UART_DATA_PROTOCOL_QUERY_H

#include <stdint.h>
#include <ThreadPool/ThreadPool.h>
#include <IO/Protocol/Data_Protocol.h>
#include <IO/Protocol/RP2040_UART/Data_Protocol/Status.h>
#include <IO/Protocol/RP2040_UART/Data_Protocol/Query_Worker.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    class Query {
        public:
            Query(Node *node, uint8_t magic);
            ~Query();
        
        protected:
            Query();

            Query_Worker *runnable_;
    };
}
#endif
