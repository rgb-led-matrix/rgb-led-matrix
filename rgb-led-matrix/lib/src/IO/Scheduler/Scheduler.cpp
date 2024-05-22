#include <thread>
#include <IO/Scheduler/Scheduler.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Unknown_Type.h>
#include <Logger/Logger.h>

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
                        Logger::get_logger()->write(Logger::Level::WARN, "Scheduler: Panel send failed to complete.");
                        Logger::get_logger()->write(Logger::Level::INFO, "Scheduler: Attempting timeout to reset node.");

                        Logger::get_logger()->write(Logger::Level::WARN, "Scheduler: Exceeded number of resend attempts.");
                        Logger::get_logger()->write(Logger::Level::INFO, "Scheduler: Attempting bootloader to reset node.");

                        Logger::get_logger()->write(Logger::Level::WARN, "Scheduler: Exceeded number of boot attempts.");
                        Logger::get_logger()->write(Logger::Level::INFO, "Scheduler: Crashing out.");
                        lock_.unlock();
                        throw String_Exception("Scheduler: We have communication failure with Panel.");
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