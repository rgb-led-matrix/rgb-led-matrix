#ifndef RP2040_UART_DATA_H
#define RP2040_UART_DATA_H

#include <stdint.h>
#include <IO/Node/Node.h>
#include <IO/Protocol/Protocol.h>
#include <ThreadPool/ThreadPool.h>
#include <IO/Protocol/RP2040_UART/Status.h>

namespace rgb_matrix {
    class Data {
        public:
            Data(Node *node);
            ~Data();

            Protocol::Status send_data(uint8_t *buf, uint32_t len);
        
        protected:
            Data();

            class Worker : public Runnable {
                public:
                    void run();

                    uint8_t *buffer;
                    uint32_t length;
                    Node *node;
                    Protocol::Status status;
            };

            Worker *runnable_;
            Status *status_;
    };
}
#endif
