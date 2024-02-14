#include "IO/Scheduler/Scheduler.h"

namespace rgb_matrix {
    Scheduler::Scheduler() {
        shutdown_ = false;
        thread_ = nullptr; 
    }

    Scheduler::~Scheduler() {
        shutdown();
    }

    bool Scheduler::start() {
        if (thread_ == nullptr) {
            shutdown_ = false;
            thread_ = new std::thread(&Scheduler::worker_thread, this);
            return true;
        }

        return false;
    }

    void Scheduler::shutdown() {
        if (thread_ != nullptr) {
            shutdown_ = true;
            thread_->join();
            delete thread_;
            thread_ = nullptr;
        }
    }

    bool Scheduler::add_node(Node *node) {
        if (thread_ == nullptr) {
            for (std::list<Node *>::iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
                if ((*it) == node)
                    return true;
            }

            nodes_.push_back(node);
            return true;
        }

        return false;
    }

    void Scheduler::worker_thread(Scheduler *object) {
        while (!object->shutdown_) {
            // Wait for sync point
            for (std::list<Node *>::iterator it = object->nodes_.begin(); it != object->nodes_.end(); ++it) {
                while((*it)->get_protocol_status() == Protocol::Status::NOT_FINISHED) {
                    // TODO: Sleep
                }
            }

            // Advance the state
            for (std::list<Node *>::iterator it = object->nodes_.begin(); it != object->nodes_.end(); ++it) {
                (*it)->acknowledge((*it)->get_protocol_status());
            }
        }
    }
}