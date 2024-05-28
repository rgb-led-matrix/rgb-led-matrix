#ifndef RP2040_UART_DATA_PROTOCOL_QUERY_H
#define RP2040_UART_DATA_PROTOCOL_QUERY_H

#include <stdint.h>
#include <ThreadPool/ThreadPool.h>
#include <IO/Protocol/Data_Protocol.h>
#include <IO/Protocol/RP2040_UART/Data_Protocol/Status.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    /*class Query_Request {
        public:
            // Input and Output are serialized data (byte stream)
            //  Implementation must be capable of deserializing frame in byte stream
            //  Implementation is responsible for ensuring network order
            virtual void *query(void *buf, uint32_t len) = 0;
    };*/

    class Query {
        public:
            Query(Node *node, uint8_t magic);
            ~Query();
        
        protected:
            Query();

            Runnable *runnable_;
    };
}
#endif
