#ifndef RP2040_UART_STATUS_H
#define RP2040_UART_STATUS_H

#include <stdint.h>
#include <thread>
#include <IO/Node/Node.h>

namespace rgb_matrix {
    class Status {
        public:
            Status(Node *node);

            uint32_t get_status();
        
        protected:
            Status();

            std::thread thread_;
    };
}
#endif
