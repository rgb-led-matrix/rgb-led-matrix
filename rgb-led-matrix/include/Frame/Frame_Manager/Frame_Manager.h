#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include <queue>
#include <stdint.h>
#include <Frame/Frame.h>

namespace rgb_matrix {
    // Optional construct for managing framerate
    class Frame_Manager {
        public:
            Frame_Manager(int framerate = 30, bool isAsync = false);
            ~Frame_Manager();

            void push_frame(Frame *frame);

        protected:
            std::queue<Frame *> frames_;
    };
}
#endif
