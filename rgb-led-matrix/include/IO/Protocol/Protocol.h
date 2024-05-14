#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <mutex>
#include <list>
#include <IO/Node/Node.h>

namespace rgb_matrix {
    enum class Protocol_Role {
        Data,
        Control
    };

    // Required construct for OSI Layer 2 and above
    //  Note these are client implementations for an internal Mediator Pattern
    // 
    //      It is recommended to keep the performance of the L1/L2 around 150uS
    //  regardless of framerate. L7 is more relaxed with something like 15mS
    //  requirements (assuming 30FPS). L1/L2 needs to make sure than all protocols/pipes
    //  (assuming MultiPanel) finish with a max delta of 150uS between fastest and slowest
    //  pipe. Otherwise frame switches may become noticable to cameras. (Assuming 3kHz
    //  is enough to hide from them.)
    class Protocol {
        public:
            Protocol(Node *node, Protocol_Role role);
            virtual ~Protocol();

            enum Status {
                NOT_FINISHED,
                ERROR,
                FINISHED
            };

            enum class Commands {
                Trigger,
                Reset,
                Acknowledge
            };

            // For Panel
            void send(uint8_t *buf, uint32_t size, uint8_t sizeof_t, uint8_t multiplex, uint8_t columns, uint8_t format);

            // For Scheduler
            Status get_protocol_status();
            void signal(Commands command);

        protected:
            Protocol();

            Status get_protocol_status(bool clear_errors);
            virtual Status internal_state_machine(bool clear_errors) = 0;
            virtual void internal_signal(Commands command) = 0;
            void claim();
            void release();

            Node *node_;
            Protocol_Role role_;
            uint8_t *buf_;
            uint32_t size_;
            uint8_t sizeof_t_;
            uint8_t multiplex_;
            uint8_t columns_;
            uint8_t format_;
            std::mutex lock_;
            std::list<Node *> list_;
            bool claim_;
    };
}
#endif
