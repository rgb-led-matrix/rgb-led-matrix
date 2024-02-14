#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

namespace rgb_matrix {
    class Node;
    
    class Protocol {
        public:
            virtual ~Protocol() {}

            virtual void send(uint8_t *buf, uint32_t size, Node *node) = 0;

            enum Status {
                MACRO_FINISHED,
                MICRO_FINISHED,
                NOT_FINISHED
            };

            // For Scheduler
            virtual Status get_protocol_status() = 0;
    };
}
#endif
