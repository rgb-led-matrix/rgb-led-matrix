#ifndef RP2040_UART_CONTROL_PROTOCOL_CONTROL_H
#define RP2040_UART_CONTROL_PROTOCOL_CONTROL_H

#include <IO/Protocol/Control_Protocol.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    class Control {
        public:
            Control(Node *node, uint8_t magic = 0xAE);

            void signal(Control_Protocol::Commands command);

        protected:
            Control();

            struct Control_Message {
                public:
                    Control_Message(Control_Protocol::Commands command, uint8_t magic);

                    uint32_t header;
                    uint8_t cmd;
                    uint16_t len;
                    uint8_t id;
                    uint32_t checksum;
                    uint32_t delimiter;

                private:
                    uint32_t compute_checksum();
            };

            void write(uint32_t val, uint8_t bits);

            rgb_matrix::Node *node_;
            uint8_t magic_;
    };
}
#endif
