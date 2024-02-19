#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <functional>

namespace rgb_matrix {
    template <typename R, typename F> class ThreadPool {
        public:
            ThreadPool();
            ~ThreadPool();

            void start(uint8_t count = 0);
            void stop();
            bool busy();

            void submit(const std::function<void(R, F)>& job, R return_args, F args);

        private:
            static void ThreadLoop(ThreadPool *object);

            struct payload {
                std::function<void(R, F)> function;
                R return_args;
                F args;
            };

            volatile bool shutdown_;
            std::mutex lock_;
            std::vector<std::thread> threads_;
            std::queue<payload *> work_queue_;
    };
}
#endif
