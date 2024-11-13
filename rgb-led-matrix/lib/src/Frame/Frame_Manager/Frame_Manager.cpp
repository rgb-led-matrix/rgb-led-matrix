#include <chrono>
#include "Frame/Frame_Manager/Frame_Manager.h"
#include "Exception/Null_Pointer.h"
#include "Exception/Illegal.h"
#include "Logger/Logger.h"

namespace rgb_matrix {
    Frame_Manager::Frame_Manager(uint16_t framerate) {
        if (framerate >= 1000)
            throw Illegal("Frame Manager: Framerate must be less than 1000.");

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
            throw Null_Pointer("Frame Manager: Cannot push null frame.");

        lock_.lock();
        frames_.push(frame);
        lock_.unlock();
    }

    void Frame_Manager::worker_thread(Frame_Manager *object) {
        //uint64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        //uint64_t range = 1000 / object->framerate_;

        while (!object->shutdown_) {
            // TODO: Implement framerate limiting logic

            object->lock_.lock();
            object->frames_.front()->show(0x12345678);
            object->frames_.pop();
            object->lock_.unlock();
        }
    }
}
