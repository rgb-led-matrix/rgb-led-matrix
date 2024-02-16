#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include <stdint.h>
#include <Panel/Event.h>
#include <IO/Scheduler/Scheduler.h>

namespace rgb_matrix {
    class Frame_Manager {
        public:
            Frame_Manager(int framerate = 30, bool isAsync = false);
            ~Frame_Manager();

            void push_frame(Event *frame);

        protected:
            list<Event *> frames_;
            Scheduler *scheduler_;
    };
}
#endif
