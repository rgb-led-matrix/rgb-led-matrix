#include <chrono>
#include <thread>
#include <IO/Scheduler/Scheduler.h>
using std::thread;

namespace rgb_matrix {
    void Scheduler::start() {
        lock_.lock();
        worker();
        lock_.unlock();
    }

    bool Scheduler::add_protocol(Protocol *protocol) {
        lock_.lock();
        for (std::list<Protocol *>::iterator it = protocols_.begin(); it != protocols_.end(); ++it) {
            if ((*it) == protocol) {
                lock_.unlock();
                return false;
            }
        }

        protocols_.push_back(protocol);
        lock_.unlock();
        return true;
    }

    void Scheduler::worker() {
        bool isFinished = true;

        while (isFinished) {
            // Wait for sync point
            for (std::list<Protocol *>::iterator it = protocols_.begin(); it != protocols_.end(); ++it) {
                while((*it)->get_protocol_status() == Protocol::Status::NOT_FINISHED) {
                    std::this_thread::sleep_for (std::chrono::seconds(1));
                }

                isFinished &= (*it)->get_protocol_status() == Protocol::Status::FINISHED;
            }

            // Advance the state
            for (std::list<Protocol *>::iterator it = protocols_.begin(); it != protocols_.end(); ++it) {
                (*it)->acknowledge((*it)->get_protocol_status());
            }
        }
    }
}