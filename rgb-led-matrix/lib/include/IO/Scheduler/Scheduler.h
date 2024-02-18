#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <list>
#include <mutex>
#include <IO/Protocol/Protocol.h>
using std::list;
using std::mutex;

namespace rgb_matrix {
    // Optional construct for syncing multiple Panels
    class Scheduler {
        public:
            void start();
            bool add_protocol(Protocol *protocol);

        protected:
            void worker();

            list<Protocol *> protocols_;
            mutex lock_;
    };
}
#endif
