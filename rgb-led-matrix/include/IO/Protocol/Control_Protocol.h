#ifndef CONTROL_PROTOCOL_H
#define CONTROL_PROTOCOL_H

#include <mutex>
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
    class Control_Protocol {
        public:
            Control_Protocol(Node *node);
            virtual ~Control_Protocol();

            enum Commands {
                Trigger,
                Reset,
                Acknowledge
            };

            // For Scheduler
            void signal(Commands commd);

        protected:
            Control_Protocol();

            virtual void internal_signal(Commands command) = 0;
            void claim();
            void release();

            Node *node_;
            std::mutex lock_;
            bool claim_;
    };
}
#endif
