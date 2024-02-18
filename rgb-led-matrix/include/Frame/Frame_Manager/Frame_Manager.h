#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include <queue>
#include <thread>
#include <mutex>
#include <stdint.h>
#include <Frame/Frame.h>
using std::thread;
using std::mutex;

namespace rgb_matrix {
    // Optional construct for managing framerate
    class Frame_Manager {
        public:
            Frame_Manager(int framerate = 30, bool isAsync = false);
            ~Frame_Manager();

            void push_frame(Frame *frame);

        protected:
            static void worker_thread(Frame_Manager *object);

            std::queue<Frame *> frames_;
            thread *thread_;
            mutex lock_;
            int framerate_;
            bool shutdown_;
            bool isAsync_;
    };
}
#endif
