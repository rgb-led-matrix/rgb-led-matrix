#ifndef RP2040_UART_DATA_PROTOCOL_DATA_H
#define RP2040_UART_DATA_PROTOCOL_DATA_H

#include <stdint.h>
#include <ThreadPool/ThreadPool.h>
#include <IO/Protocol/Data_Protocol.h>
#include <IO/Protocol/RP2040_UART/Data_Protocol/Status.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    class Data {
        public:
            Data(Node *node, uint8_t magic);
            ~Data();

            Data_Protocol::Status send_data(void *buf, uint32_t len, uint8_t sizeof_t, uint8_t multiplex, uint8_t columns, Data_Format_ID format);
            void clear_errors();
        
        protected:
            Data();

            class Worker : public Runnable {
                public:
                    Worker(uint8_t magic);
                    ~Worker();

                    void run();

                    uint8_t *buffer;
                    uint32_t length;
                    uint8_t sizeof_t;
                    uint8_t multiplex;
                    uint8_t columns;
                    uint8_t format;
                    rgb_matrix::Node *node;
                    rgb_matrix::Data_Protocol::Status status;
                    uint8_t magic;
                
                private:
                    Status *status_msg_;

            };

            Worker *runnable_;
    };
}
#endif
