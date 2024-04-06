#ifndef RP2040_UART_H
#define RP2040_UART_H

#include <IO/Protocol/Protocol.h>
#include <mutex>

namespace rgb_matrix {
    class RP2040_UART : public Protocol {
        public:
            RP2040_UART(Node *node);
        
        protected:
            struct frame_head {
                uint32_t preamble;
                uint8_t cmd[2];
                uint16_t len;
            };

            struct frame_tail {
                uint32_t checksum;
                uint32_t delimiter;
            };

            RP2040_UART();

            void write(frame_head *data);
            void write(frame_tail *data);
            void read(frame_head *data);
            void read(frame_tail *data);
            Status internal_state_machine();

            std::mutex lock_;
            char prev_;
    };
}
#endif
