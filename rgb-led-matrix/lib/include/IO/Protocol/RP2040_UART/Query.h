#ifndef RP2040_UART_QUERY_H
#define RP2040_UART_QUERY_H

#include <stdint.h>
#include <IO/Node/Node.h>
#include <IO/Protocol/Protocol.h>
#include <ThreadPool/ThreadPool.h>
#include <IO/Protocol/RP2040_UART/Status.h>

namespace rgb_matrix {
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
                    Protocol::Status status;
                    uint8_t magic;
                
                private:
                    Status *status_msg_;

            };

            Worker *runnable_;
    };
}
#endif
