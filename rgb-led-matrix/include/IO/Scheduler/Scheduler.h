#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <list>
#include <thread>
#include <mutex>
#include <IO/Protocol/Protocol.h>
using std::list;
using std::thread;

namespace rgb_matrix {
    class Scheduler {
        public:
            Scheduler();
            virtual ~Scheduler();

            virtual bool add_protocol(Protocol *protocol);
            bool start();
            void shutdown();

        protected:
            static void worker_thread(Scheduler *object);

            list<Protocol *> protocols_;
            thread *thread_;
            std::mutex lock_;
            bool shutdown_;
    };
}
#endif
