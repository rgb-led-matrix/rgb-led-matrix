#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <IO/Node/Node.h>
#include <ThreadPool/ThreadPool.h>

namespace rgb_matrix {
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
            Protocol(Node *node, ThreadPool<volatile bool *, void *> *pool = nullptr);
            virtual ~Protocol() {}

            enum Status {
                NOT_FINISHED,
                NEXT,
                FINISHED
            };

            // For Panel
            void send(uint8_t *buf, uint32_t size, uint8_t scan);

            // For Scheduler
            Status get_protocol_status();
            void acknowledge();

        protected:
            Protocol();

            ThreadPool<volatile bool *, void *> *get_threadpool();
            virtual Status internal_state_machine() = 0;

            Node *node_;
            uint8_t *buf_;
            uint32_t size_;
            volatile Status status_;
            uint8_t scan_;
            uint32_t counter_;
            uint8_t state_;
            ThreadPool<volatile bool *, void *> *custom_pool_;

        private:
            static ThreadPool<volatile bool *, void *> *get_static_threadpool();

            static ThreadPool<volatile bool *, void *> *pool_;
    };
}
#endif
