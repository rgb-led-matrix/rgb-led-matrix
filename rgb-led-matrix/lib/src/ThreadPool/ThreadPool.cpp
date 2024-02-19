#include <ThreadPool/ThreadPool.h>
#include <Panel/MultiPanel_Internal.h>

namespace rgb_matrix {
    template <typename R, typename F> ThreadPool<R, F>::ThreadPool() {
        shutdown_ = false;
    }

    template <typename R, typename F> ThreadPool<R, F>::~ThreadPool() {
        stop();
    }

    template <typename R, typename F> void ThreadPool<R, F>::start(uint8_t count) {
        if (count == 0) {
            count = std::thread::hardware_concurrency();

            if (count == 0)
                count = 1;
        }

        shutdown_ = false;
        for (uint8_t i = 0; i < count; i++) {
            threads_.emplace_back(std::thread(&ThreadPool::ThreadLoop,this));
        }
    }

    template <typename R, typename F> void ThreadPool<R, F>::stop() {
        shutdown_ = true;
        // TODO: Join threads
        // TODO: Clear out work_queue_;
    }

    template <typename R, typename F> bool ThreadPool<R, F>::busy() {
        bool result;

        lock_.lock();
        result = !work_queue_.empty();
        lock_.unlock();

        return result;
    }

    template <typename R, typename F> void ThreadPool<R, F>::submit(const std::function<void(R, F)>& job, R return_args, F args) {
        payload *p = new payload();

        p->function = job;
        p->return_args = return_args;
        p->args = args;

        lock_.lock();
        work_queue_.push(p);
        lock_.unlock();
    }

    // Probably should use conditional but the interface is silly so polling instead.
    template <typename R, typename F> void ThreadPool<R, F>::ThreadLoop(ThreadPool<R, F> *object) {
        payload *p;

        while(!object->shutdown_) {
            while (!object->busy())
                std::this_thread::sleep_for(std::chrono::seconds(1));
            
            object->lock_.lock();
            p = object->work_queue_.front();
            object->work_queue_.pop();
            object->lock_.unlock();

            p->function(p->return_args, p->args);
            delete p;
        }
    }

    template class ThreadPool<void *, MultiPanel_Internal::show_packet>;
    template class ThreadPool<void *, MultiPanel_Internal::set_brightness_packet>;
    template class ThreadPool<void *, MultiPanel_Internal::map_wavelength_packet>;
}