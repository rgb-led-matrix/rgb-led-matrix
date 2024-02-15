#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include <stdint.h>
#include <Panel/MultiPanel_Mapper.h>
#include <IO/Scheduler/Scheduler.h>

namespace rgb_matrix {
    class Frame_Manager {
        public:
            Frame_Manager(int framerate = 30, bool isAsync = false);
            virtual ~Frame_Manager();

            void add_frame(MultiPanel_Mapper *frame);

        protected:
            list<MultiPanel_Mapper *> frames_;
            Scheduler *scheduler_;
    };
}
#endif
