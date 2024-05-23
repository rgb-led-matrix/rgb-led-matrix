#include <thread>
#include <algorithm>
#include <ThreadPool/ThreadPool.h>
#include <Panel/MultiPanel_Internal.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    ThreadPool *ThreadPool::pool_[2] = { nullptr };

    ThreadPool::ThreadPool() {
        uint8_t count = std::max(std::thread::hardware_concurrency() / 2, (unsigned int) 1);

        for (uint8_t i = 0; i < count; i++)
            threads_.emplace_back(std::thread(&ThreadPool::ThreadLoop, this));
    }

    void ThreadPool::submit(Runnable *t) {
        if (t == nullptr)
            throw Null_Pointer("ThreadPool: Cannot submit null runnable.");

        lock_.lock();
        work_queue_.push(t);
        conditional_.notify_one();
        lock_.unlock();
    }

    void ThreadPool::ThreadLoop(ThreadPool *object) {
        while(true) {
            std::unique_lock<std::mutex> lk(object->lock_);
            object->conditional_.wait(lk, [object]{ return !object->work_queue_.empty(); });
            Runnable *t = object->work_queue_.front();
            object->work_queue_.pop();
            lk.unlock();

            t->run();
        }
    }

    ThreadPool *ThreadPool::get_threadpool(Pool_ID id) {
        ThreadPool **result;

        switch (id) {
            case Pool_ID::Drawer:
                result = &pool_[0];
                break;
            case Pool_ID::IO:
                result = &pool_[1];
                break;
            default:
                throw Unknown_Type("ThreadPool: Pool ID is not defined.");
                break;
        }

        if (*result == nullptr)
            *result = new ThreadPool();

        return *result;
    }
}