#include <thread>
#include <IO/Scheduler/Scheduler.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    void Scheduler::start(Control_Protocol *control) {
        bool isFinished = false;

        if (control == nullptr)
            throw Null_Pointer("Protocol");

        lock_.lock();
        while (!isFinished) {
            isFinished = true;

            for (std::list<Data_Protocol *>::iterator it = protocols_.begin(); it != protocols_.end(); ++it) {
                switch ((*it)->get_protocol_status()) {
                    case Data_Protocol::Status::FINISHED:
                        isFinished &= true;
                        break;
                    case Data_Protocol::Status::NOT_FINISHED:
                        isFinished = false;
                        break;
                    case Data_Protocol::Status::ERROR:
                        isFinished = false;
                        // TODO: Error handle as required
                        lock_.unlock();
                        throw String_Exception("Houstin we have a problem");
                        break;
                    default:
                        lock_.unlock();
                        throw Unknown_Type("Status");
                        break;
                }
            }
        }
        control->signal(Control_Protocol::Commands::Trigger);
        lock_.unlock();
    }

    bool Scheduler::add_protocol(Data_Protocol *protocol) {
        if (protocol == nullptr)
            throw Null_Pointer("Protocol");

        lock_.lock();
        for (std::list<Data_Protocol *>::iterator it = protocols_.begin(); it != protocols_.end(); ++it) {
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