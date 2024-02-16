#include "IO/Scheduler/Scheduler.h"

namespace rgb_matrix {
    Scheduler::Scheduler() {
        shutdown_ = false;
        isFinished_ = false;
        thread_ = nullptr; 
    }

    Scheduler::~Scheduler() {
        shutdown();
    }

    bool Scheduler::start() {
        lock_.lock();

        if (thread_ == nullptr) {
            shutdown_ = false;
            isFinished_ = false;
            thread_ = new std::thread(&Scheduler::worker_thread, this);

            lock_.unlock();
            return true;
        }

        lock_.unlock();
        return false;
    }

    void Scheduler::shutdown() {
        lock_.lock();
        if (thread_ != nullptr) {
            shutdown_ = true;
            thread_->join();
            delete thread_;
            thread_ = nullptr;
        }
        lock_.unlock();
    }

    bool Scheduler::isFinished() {
        return isFinished_;
    }

    bool Scheduler::add_protocol(Protocol *protocol) {
        lock_.lock();
        if (thread_ == nullptr) {
            for (std::list<Protocol *>::iterator it = protocols_.begin(); it != protocols_.end(); ++it) {
                if ((*it) == protocol) {
                    lock_.unlock();
                    return true;
                }
            }

            protocols_.push_back(protocol);
            lock_.unlock();
            return true;
        }

        lock_.unlock();
        return false;
    }

    void Scheduler::worker_thread(Scheduler *object) {
        while (!object->shutdown_) {
            bool isFinished = true;

            // Wait for sync point
            for (std::list<Protocol *>::iterator it = object->protocols_.begin(); it != object->protocols_.end(); ++it) {
                while((*it)->get_protocol_status() == Protocol::Status::NOT_FINISHED) {
                    // TODO: Sleep
                }

                isFinished &= (*it)->get_protocol_status() == Protocol::Status::FINISHED;
            }

            // Advance the state
            for (std::list<Protocol *>::iterator it = object->protocols_.begin(); it != object->protocols_.end(); ++it) {
                (*it)->acknowledge((*it)->get_protocol_status());
            }

            if (isFinished) {
                object->isFinished_ = true;
                break;
            }
        }
    }
}