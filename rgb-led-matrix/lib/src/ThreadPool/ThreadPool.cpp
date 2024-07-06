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

        for (uint8_t i = 0; i < count; i += 2) {
            if ((count - 1) > 0)
                threads_.emplace_back(new ThreadDomain(2));
            else
                threads_.emplace_back(new ThreadDomain(1));
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