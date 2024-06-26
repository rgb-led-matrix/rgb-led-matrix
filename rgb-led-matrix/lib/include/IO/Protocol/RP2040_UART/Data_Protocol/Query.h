#ifndef RP2040_UART_DATA_PROTOCOL_QUERY_H
#define RP2040_UART_DATA_PROTOCOL_QUERY_H

#include <stdint.h>
#include <ThreadPool/ThreadPool.h>
#include <IO/Protocol/Data_Protocol.h>
#include <IO/Protocol/RP2040_UART/Data_Protocol/Status.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    class Query {
        public:
            Query(Node *node, uint8_t magic);
            ~Query();
        
        protected:
            Query();

            class Worker : public Runnable {
                public:
                    Worker(uint8_t magic);
                    ~Worker();

                    void run();

                    Node *node;
                    Data_Protocol::Status status;
                    uint8_t magic;
                
                private:
                    Status *status_msg_;

            };

            Worker *runnable_;
    };
}
#endif
