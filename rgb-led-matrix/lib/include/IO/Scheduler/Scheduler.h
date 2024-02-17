#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <list>
#include <thread>
#include <mutex>
#include <IO/Protocol/Protocol.h>
using std::list;
using std::thread;

namespace rgb_matrix {
    // Optional construct for syncing multiple Panels
    class Scheduler {
        public:
            Scheduler();
            ~Scheduler();

            bool start();
            void shutdown();
            bool isFinished();

            bool add_protocol(Protocol *protocol);

        protected:
            static void worker_thread(Scheduler *object);

            list<Protocol *> protocols_;
            thread *thread_;
            std::mutex lock_;
            bool shutdown_;
            bool isFinished_;
    };
}
#endif
