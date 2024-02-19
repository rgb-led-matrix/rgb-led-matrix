#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace rgb_matrix {
    template <typename R, typename... F> class ThreadPool {
        public:
            void start(uint8_t count = 0);
            void stop();
            bool busy();

            void submit(const std::function<void(R, F...)>& job, R, F...);

        private:
            static void ThreadLoop(ThreadPool *object);

            bool should_terminate;
            std::mutex queue_mutex;
            std::condition_variable mutex_condition;
            std::vector<std::thread> threads;
            std::queue<std::function<void(R, F...)>> jobs;
    };
}
#endif
