#include <algorithm>
#include "ThreadPool/ThreadPool.h"
#include "Exception/Null_Pointer.h"
#include "Exception/Unknown_Type.h"

namespace rgb_matrix {
    ThreadPool *ThreadPool::pool_[2] = { nullptr };

    // Note: Threads are scheduled using FIFO (batch policy)
    //  Time share may exist in a premptive manner based on pool priority (thread priority)
    ThreadPool::ThreadPool(ThreadDomain::ThreadType type, uint8_t count, uint8_t priority) {
        uint8_t step;

        // Allow SMT domains for Compute Threads, SMP domains for everything else.
        //  Most processors are limited to two by hyperthreading.
        //      Some ARM processors allow up to 4 or 8 threads per domain.
        switch (type) {
            case ThreadDomain::ThreadType::Compute:         // Compute Bound (Few load/stores - MISD for example)
                // Future: Support SMT-4?
                step = 2;
                break;
            case ThreadDomain::ThreadType::Standard:        // Memory Bound (Capacity drives load/stores, either data or instruction - MIMD for example)
                // Future: Support HyperThreading?
            case ThreadDomain::ThreadType::IO:              // IO Bound (Purely load/stores - SISD and SIMD for example)
            default:
                step = 1;
                break;
        }

        //  We do not attempt to throttle against system load.
        //      We use priority and trust the OS instead.
        for (uint8_t i = 0; i < count; i += step) {
            if ((count - i) > step)
                threads_.emplace_back(new ThreadDomain(step, type, priority));
            else
                threads_.emplace_back(new ThreadDomain(count - i, type, priority));     // Future: Schedule a dummy thread beside it?
        }
    }

    void ThreadPool::submit(Runnable *t) {
        uint8_t num = 1;
        bool run = true;

        if (t == nullptr)
            throw Null_Pointer("ThreadPool: Cannot submit null runnable.");

        lock_.lock();
        while (run) {
            for (size_t i = 0; i < threads_.size(); i++) {
                if (threads_.at(i)->pending_num() < num) {
                    threads_.at(i)->submit(t);
                    run = false;
                    break;
                }
            }

            ++num;
        }
        lock_.unlock();
    }

    ThreadPool *ThreadPool::get_threadpool(Pool_ID id) {
        ThreadPool **result;
        uint8_t priority = 0;
        uint8_t count = 1;
        ThreadDomain::ThreadType type = ThreadDomain::ThreadType::Standard;

        switch (id) {
            case Pool_ID::Drawer:
                result = &pool_[0];
                type = ThreadDomain::ThreadType::Standard;
                count = 2;
                break;
            case Pool_ID::IO:
                priority = 250;
                result = &pool_[1];
                type = ThreadDomain::ThreadType::IO;
                break;
            default:
                throw Unknown_Type("ThreadPool: Pool ID is not defined.");
                break;
        }

        if (*result == nullptr)
            *result = new ThreadPool(type, count, priority);

        return *result;
    }
}