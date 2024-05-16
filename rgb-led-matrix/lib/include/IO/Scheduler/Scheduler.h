#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <list>
#include <mutex>
#include <IO/Protocol/Control_Protocol.h>
#include <IO/Protocol/Data_Protocol.h>

namespace rgb_matrix {
    // Uses Mediator Pattern to synchronize panels in MultiPanel::show
    class Scheduler {
        public:
            void start(Control_Protocol *control);
            bool add_protocol(Data_Protocol *protocol);

        protected:
            std::list<Data_Protocol *> protocols_;
            std::mutex lock_;
    };
}
#endif
