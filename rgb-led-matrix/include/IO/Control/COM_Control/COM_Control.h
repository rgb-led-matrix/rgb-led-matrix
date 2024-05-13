#ifndef COM_CONTROL_H
#define COM_CONTROL_H

#include <IO/Control/Control.h>

namespace rgb_matrix {
    class COM_Control : public Control {
        public:
            COM_Control(Node *node, uint8_t magic = 0xAE);

            void signal(Commands command);

        protected:
            COM_Control();

            struct Control_Message {
                public:
                    Control_Message(Commands command, uint8_t magic);

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

            uint8_t magic_;
    };
}
#endif
