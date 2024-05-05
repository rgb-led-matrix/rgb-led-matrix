#ifndef RP2040_UART_STATUS_H
#define RP2040_UART_STATUS_H

#include <stdint.h>
#include <thread>
#include <IO/Node/Node.h>

namespace rgb_matrix {
    class Status {
        public:
            Status(Node *node);

            enum class STATUS {
                IDLE_0,
                IDLE_1,
                ACTIVE_0,
                ACTIVE_1,
                READY
            };

            bool get_status(STATUS current, STATUS expected);
            STATUS get_status();
        
        protected:
            Status();

            STATUS translate_id(uint32_t id);

            std::thread thread_;
            Node *node_;
    };
}
#endif
