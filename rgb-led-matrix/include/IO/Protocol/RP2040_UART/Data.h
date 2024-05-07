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

            Protocol::Status send_data(uint8_t *buf, uint32_t len, uint8_t sizeof_t, uint8_t multiplex, uint8_t columns, uint8_t format);
            void clear_errors();
        
        protected:
            Data();

            class Worker : public Runnable {
                public:
                    Worker();
                    ~Worker();

                    void run();

                    uint8_t *buffer;
                    uint32_t length;
                    uint8_t sizeof_t;
                    uint8_t multiplex;
                    uint8_t columns;
                    uint8_t format;
                    Node *node;
                    Protocol::Status status;
                
                private:
                    Status *status_msg_;

            };

            Worker *runnable_;
    };
}
#endif
