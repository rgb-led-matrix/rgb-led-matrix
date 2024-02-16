#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include <queue>
#include <stdint.h>
#include <Panel/Event.h>

namespace rgb_matrix {
    // Optional construct for managing framerate
    class Frame_Manager {
        public:
            Frame_Manager(int framerate = 30, bool isAsync = false);
            ~Frame_Manager();

            void push_frame(Event *frame);

        protected:
            std::queue<Event *> frames_;
    };
}
#endif
