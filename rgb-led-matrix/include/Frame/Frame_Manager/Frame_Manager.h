#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include <queue>
#include <thread>
#include <mutex>
#include <Frame/Frame.h>

namespace rgb_matrix {
    // Optional construct for managing framerate
    class Frame_Manager {
        public:
            Frame_Manager(uint16_t framerate = 30, bool isAsync = false);
            ~Frame_Manager();

            void push_frame(Frame *frame);

        protected:
            static void worker_thread(Frame_Manager *object);

            std::queue<Frame *> frames_;
            std::thread *thread_;
            std::mutex lock_;
            uint16_t framerate_;
            bool shutdown_;
            bool isAsync_;
    };
}
#endif
