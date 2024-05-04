#include <ThreadPool/ThreadPool.h>
#include <Panel/MultiPanel_Internal.h>

namespace rgb_matrix {
    template <typename R, typename F> void ThreadPool<R, F>::start(uint8_t count) {
        for (uint8_t i = 0; i < count; i++)
            threads_.emplace_back(std::thread(&ThreadPool::ThreadLoop, this));
    }

    template <typename R, typename F> void ThreadPool<R, F>::submit(const std::function<void(R, F)>& job, R return_args, F args) {
        payload *p = new payload();

        p->function = job;
        p->return_args = return_args;
        p->args = args;

        lock_.lock();
        work_queue_.push(p);
        conditional_.notify_one();
        lock_.unlock();
    }

    template <typename R, typename F> void ThreadPool<R, F>::ThreadLoop(ThreadPool<R, F> *object) {
        payload *p;

        while(true) {
            std::unique_lock<std::mutex> lk(object->lock_);
            object->conditional_.wait(lk, [object]{ return !object->work_queue_.empty(); });
            p = object->work_queue_.front();
            object->work_queue_.pop();
            lk.unlock();

            p->function(p->return_args, p->args);
            delete p;
        }
    }

    template class ThreadPool<volatile bool *, void *>;
    template class ThreadPool<volatile bool *, MultiPanel_Internal::show_packet>;
    template class ThreadPool<volatile bool *, MultiPanel_Internal::set_brightness_packet>;
    template class ThreadPool<volatile bool *, MultiPanel_Internal::map_wavelength_packet>;
}