#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace rgb_matrix {
    template <typename R, typename F> class ThreadPool {
        public:
            void start(uint8_t count = 0);
            void submit(const std::function<void(R, F)>& job, R return_args, F args);

        private:
            static void ThreadLoop(ThreadPool *object);
            bool busy();

            struct payload {
                std::function<void(R, F)> function;
                R return_args;
                F args;
            };

            std::mutex lock_;
            std::condition_variable conditional_;
            std::vector<std::thread> threads_;
            std::queue<payload *> work_queue_;
    };
}
#endif
