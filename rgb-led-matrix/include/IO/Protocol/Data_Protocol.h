#ifndef DATA_PROTOCOL_H
#define DATA_PROTOCOL_H

#include <mutex>
#include <list>
#include <IO/Node/Node.h>

// Required construct for OSI Layer 2 and above
// 
//      It is recommended to keep the performance of the L1/L2 around 150uS
//  regardless of framerate. L7 is more relaxed with something like 15mS
//  requirements (assuming 30FPS). L1/L2 needs to make sure than all protocols/pipes
//  (assuming MultiPanel) finish with a max delta of 150uS between fastest and slowest
//  pipe. Otherwise frame switches may become noticable to cameras. (Assuming 3kHz
//  is enough to hide from them.)
namespace rgb_matrix {
    // Note these are client implementations for an internal Mediator Pattern
    class Data_Protocol {
        public:
            Data_Protocol(Node *node);
            virtual ~Data_Protocol();

            enum Status {
                NOT_FINISHED,
                ERROR,
                FINISHED
            };

            // For Panel
            void send(uint8_t *buf, uint32_t size, uint8_t sizeof_t, uint8_t multiplex, uint8_t columns, uint8_t format);

            // For Scheduler
            Status get_protocol_status();

        protected:
            Data_Protocol();

            Status get_protocol_status(bool clear_errors);
            virtual Status internal_state_machine(bool clear_errors) = 0;
            void claim();
            void release();

            Node *node_;
            uint8_t *buf_;
            uint32_t size_;
            uint8_t sizeof_t_;
            uint8_t multiplex_;
            uint8_t columns_;
            uint8_t format_;
            std::mutex lock_;
            bool claim_;
    };
}
#endif
