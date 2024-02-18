#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <list>
#include <mutex>
#include <IO/Protocol/Protocol.h>
using std::list;
using std::mutex;

namespace rgb_matrix {
    class Scheduler {
        public:
            void start();
            bool add_protocol(Protocol *protocol);

        protected:
            list<Protocol *> protocols_;
            mutex lock_;
    };
}
#endif
