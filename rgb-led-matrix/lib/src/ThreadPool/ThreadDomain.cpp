#include "ThreadPool/ThreadPool.h"
#include "ThreadPool/ThreadDomain.h"
#include "Exception/Null_Pointer.h"

namespace rgb_matrix {
    // Note: This may need to change for SMT under Intel Rentable units or ARM SMT!
    //  The usage of a compute thread with IO thread inside a single domain is not expected by this interface
    //  I expect Intel's rentable units to fail, and ARM to implement two or four threads per cluster of uniform type
    //  I also expect the OS to take over pinning at that point due to the number of cores, but this interface should play nice
    ThreadDomain::ThreadDomain(uint8_t thread_count, ThreadType type, uint8_t priority, ThreadPolicy policy, int8_t pool_num) {
        // Future: Pin to specific cores or domains?
        for (uint8_t i = 0; i < thread_count; i++)
            threads_.emplace_back(std::thread(&ThreadDomain::ThreadLoop, this));
    }

    void ThreadDomain::submit(Runnable *t) {
        if (t == nullptr)
            throw Null_Pointer("ThreadDomain: Cannot submit null runnable.");

        lock_.lock();
        work_queue_.push(t);
        conditional_.notify_one();
        lock_.unlock();
    }

    uint8_t ThreadDomain::pending_num() {
        uint8_t result = 0;
        lock_.lock();
        result = work_queue_.size();
        lock_.unlock();
        return result;
    }

    void ThreadDomain::ThreadLoop(ThreadDomain *object) {
        while(true) {
            std::unique_lock<std::mutex> lk(object->lock_);
            object->conditional_.wait(lk, [object]{ return !object->work_queue_.empty(); });
            Runnable *t = object->work_queue_.front();
            object->work_queue_.pop();
            lk.unlock();

            try {
                t->run();
            }
            catch (...) {
                // TODO: Figure something out
            }
        }
    }
}