#include <chrono>
#include <thread>
#include <IO/Scheduler/Scheduler.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    void Scheduler::start() {
        bool isNext, isFinished = false;

        lock_.lock();
        while (!isFinished) {
            isFinished = true;
            isNext = true;

            // Wait for sync point
            for (std::list<Protocol *>::iterator it = protocols_.begin(); it != protocols_.end(); ++it) {
                while((*it)->get_protocol_status() == Protocol::Status::NOT_FINISHED) {
                    std::this_thread::sleep_for (std::chrono::seconds(1));
                }

                switch ((*it)->get_protocol_status()) {
                    case Protocol::Status::FINISHED:
                        isFinished &= true;
                        isNext &= true;
                        break;
                    case Protocol::Status::NOT_FINISHED:
                        isFinished &= false;
                        isNext &= false;
                        (*it)->acknowledge();
                        break;
                    case Protocol::Status::NEXT:
                        isNext &= true;
                        break;
                    default:
                        throw Unknown_Type("Status");
                        break;
                }
            }

            // Advance the state
            if (isNext)
                for (std::list<Protocol *>::iterator it = protocols_.begin(); it != protocols_.end(); ++it) {
                    (*it)->acknowledge();
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