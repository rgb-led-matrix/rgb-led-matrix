#ifndef THREADDOMAIN_H
#define THREADDOMAIN_H

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace rgb_matrix {
    class Runnable;

    class ThreadDomain {
        public:
            enum class ThreadType {
                Compute,
                Standard
            };

            ThreadDomain(uint8_t thread_count = 2, ThreadType type = ThreadType::Standard, uint8_t priority = 0, int8_t pool_num = -1);
            
            void submit(Runnable *t);
            uint8_t pending_num();

        private:
            static void ThreadLoop(ThreadDomain *object);

            std::mutex lock_;
            std::condition_variable conditional_;
            std::vector<std::thread> threads_;
            std::queue<Runnable *> work_queue_;
    };
}
#endif
