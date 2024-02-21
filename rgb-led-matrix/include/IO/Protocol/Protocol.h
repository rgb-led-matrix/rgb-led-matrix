#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <IO/Node/Node.h>
#include <mutex>

namespace rgb_matrix {
    // Required construct for OSI Layer 2 and above
    //  Note these are client implementations for an internal Mediator Pattern
    //  Warning a glitch is possible and there really is no solution in software
    //      We send out a packet of information and we expect this to arrive at
    //  the same time. It is like a foot race, we attempt to have all racers
    //  start the race against each other with a single shot. Should their really
    //  be multiple shots to start the race it is not much of a competition as
    //  the one who starts with the first shot is very much likely to win. The only
    //  real solution we can do on the software side is try to make these multiple
    //  shots take place in rapid succession. If so they should not have a huge
    //  impact on the outcome of the race. However should there be road blocks
    //  in some racers path we cannot mitigate this. Hardware should attempt
    //  to give each racer a clean path before hand.
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
