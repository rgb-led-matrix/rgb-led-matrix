#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <IO/Node/Node.h>
#include <mutex>

namespace rgb_matrix {
    // Required construct for OSI Layer 2 and above
    //    Note these are client implementations for an internal Mediator Pattern
    class Protocol {
        public:
            Protocol(Node *node);
            virtual ~Protocol() {}

            enum Status {
                NOT_FINISHED,
                NEXT,
                FINISHED
            };

            // For Panel
            void send(uint8_t *buf, uint32_t size);

            // For Scheduler
            Status get_protocol_status();
            void acknowledge();

        protected:
            Protocol();

            virtual Status internal_state_machine() = 0;

            Node *node_;
            uint8_t *buf_;
            uint32_t size_;
            volatile Status status_;
            std::mutex lock_;
    };
}
#endif
