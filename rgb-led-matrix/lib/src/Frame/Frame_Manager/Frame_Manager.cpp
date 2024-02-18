#include <chrono>
#include <Frame/Frame_Manager/Frame_Manager.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    Frame_Manager::Frame_Manager(uint16_t framerate, bool isAsync) {
        if (framerate >= 1000)
            throw Illegal("Framerate");

        isAsync_ = isAsync;
        framerate_ = framerate;
        shutdown_ = false;
        thread_ = new std::thread(&Frame_Manager::worker_thread, this);
    }

    Frame_Manager::~Frame_Manager() {
        shutdown_ = true;
        thread_->join();
        delete thread_;
    }

    void Frame_Manager::push_frame(Frame *frame) {
        if (frame == nullptr)
            throw Null_Pointer("Frame");

        lock_.lock();
        frame->isFree_ = false;
        frames_.push(frame);
        lock_.unlock();
    }

    void Frame_Manager::worker_thread(Frame_Manager *object) {
        uint64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        uint64_t range = 1000 / object->framerate_;

        while (!object->shutdown_) {
            if (object->isAsync_) {
                if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - time) > range) {
                    if (!object->frames_.empty())
                        object->frames_.pop();

                    time += range;
                }

                time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            }

            object->lock_.lock();
            Frame *f = object->frames_.front();
            object->lock_.unlock();

            if (f->isMulti_)
                f->multi_->show();
            else
                f->single_->show(f->protocol_);

            object->lock_.lock();
            f->isFree_ = true;
            object->frames_.pop();
            object->lock_.unlock();
        }
    }
}
