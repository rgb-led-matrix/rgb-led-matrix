#include "IO/Scheduler/Scheduler.h"

namespace rgb_matrix {
    Scheduler::Scheduler() {
        shutdown_ = false;
        thread_ = new std::thread(&Scheduler::worker_thread, this);
    }

    Scheduler::~Scheduler() {
        shutdown_ = true;
        thread_->join();
        delete thread_;
    }

    void Scheduler::add_node(Node *node) {
        for (std::list<Node *>::iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
            if ((*it) == node)
                return;
        }

        nodes_.push_back(node);
    }

    void Scheduler::worker_thread(Scheduler *object) {
        while (!object->shutdown_) {
            for (std::list<Node *>::iterator it = object->nodes_.begin(); it != object->nodes_.end(); ++it) {
                (*it)->process(200);
            }
        }
    }
}