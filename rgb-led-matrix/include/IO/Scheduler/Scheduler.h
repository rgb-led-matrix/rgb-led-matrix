#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <list>
#include <thread>
#include <IO/Node/Node.h>
using std::list;
using std::thread;

namespace rgb_matrix {
    class Scheduler {
        public:
            Scheduler();
            virtual ~Scheduler();

            virtual void add_node(Node *node);

        protected:
            static void worker_thread(Scheduler *object);

            list<Node *> nodes_;
            thread *thread_;
            bool shutdown_;
    };
}
#endif
