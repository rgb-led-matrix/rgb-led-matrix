#include <chrono>
#include <thread>
#include <IO/Scheduler/Scheduler.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    void Scheduler::start(Control *control) {
        bool isFinished = false;

        lock_.lock();
        while (!isFinished) {
            isFinished = true;

            for (std::list<Protocol *>::iterator it = protocols_.begin(); it != protocols_.end(); ++it) {
                switch ((*it)->get_protocol_status()) {
                    case Protocol::Status::FINISHED:
                        isFinished &= true;
                        break;
                    case Protocol::Status::NOT_FINISHED:
                        isFinished = false;
                        break;
                    case Protocol::Status::ERROR:
                        isFinished = false;
                        // TODO: Verify that we have moved to the correct state
                        //  Error handle as required
                        break;
                    default:
                        lock_.unlock();
                        throw Unknown_Type("Status");
                        break;
                }
            }
        }
        control->signal(Control::Commands::Trigger);
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