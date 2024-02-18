#include <chrono>
#include <thread>
#include <IO/Scheduler/Scheduler.h>
#include <Exception/Null_Pointer.h>

namespace rgb_matrix {
    void Scheduler::start() {
        bool isFinished = true;

        lock_.lock();
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
        lock_.unlock();
    }

    bool Scheduler::add_protocol(Protocol *protocol) {
        if (protocol == nullptr)
            throw Null_Pointer("Protocol");

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
}