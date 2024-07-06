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
            ThreadDomain(uint8_t num = 2);
            
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
