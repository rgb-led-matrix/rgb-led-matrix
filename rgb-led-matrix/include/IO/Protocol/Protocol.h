#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <IO/Node/Node.h>

namespace rgb_matrix {    
    class Protocol {
        public:
            Protocol(Node *node);
            virtual ~Protocol() {}

            enum Status {
                MACRO_FINISHED,
                MICRO_FINISHED,
                NOT_FINISHED
            };

            // For Panel
            virtual void send(uint8_t *buf, uint32_t size) = 0;

            // For Scheduler
            virtual Status get_protocol_status() = 0;
            virtual void acknowledge(Status) = 0;

        protected:
            Protocol();

            Node *node_;
    };
}
#endif
