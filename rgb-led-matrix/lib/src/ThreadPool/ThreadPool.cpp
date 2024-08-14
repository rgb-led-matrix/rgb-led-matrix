#include <algorithm>
#include <ThreadPool/ThreadPool.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    ThreadPool *ThreadPool::pool_[2] = { nullptr };

    // Note: Threads are scheduled using FIFO (batch policy)
    //  Time share may exist in a premptive manner based on pool priority (thread priority)
    ThreadPool::ThreadPool(ThreadDomain::ThreadType type, uint8_t priority) {
        uint8_t count = std::max(std::thread::hardware_concurrency() / 2, (unsigned int) 1);

        if (count == 1) {
            threads_.emplace_back(new ThreadDomain(1, type, priority));   // Future: Schedule a dummy thread beside it?
        }
        else {
            // Most processors are limited to two by hyperthreading.
            for (uint8_t i = 0; i < count; i += 2) {
                switch (type) {
                    case ThreadDomain::ThreadType::Compute:
                        threads_.emplace_back(new ThreadDomain(2, type, priority));
                        break;
                    case ThreadDomain::ThreadType::Standard:
                    case ThreadDomain::ThreadType::IO:
                    default:
                        threads_.emplace_back(new ThreadDomain(1, type, priority));
                        break;
                }   
            }
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
        ThreadDomain::ThreadType type = ThreadDomain::ThreadType::Standard;

        switch (id) {
            case Pool_ID::Drawer:
                result = &pool_[0];
                type = ThreadDomain::ThreadType::Compute;
                break;
            case Pool_ID::IO:
                priority = 250;
                result = &pool_[1];
                break;
            default:
                throw Unknown_Type("ThreadPool: Pool ID is not defined.");
                break;
        }

        if (*result == nullptr)
            *result = new ThreadPool(type, priority);

        return *result;
    }
}