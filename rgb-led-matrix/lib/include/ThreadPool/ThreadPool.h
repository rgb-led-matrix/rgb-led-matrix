#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace rgb_matrix {
    class Thread {
        public:
            Thread() {}
            virtual ~Thread() {}

            virtual void run() = 0;
    };

    class ThreadPool {
        public:
            virtual ~ThreadPool();

            static ThreadPool *get_threadpool();
            void submit(Thread *t);

        private:
            ThreadPool();

            static void ThreadLoop(ThreadPool *object);

            std::mutex lock_;
            std::condition_variable conditional_;
            std::vector<std::thread> threads_;
            std::queue<Thread *> work_queue_;
            static ThreadPool *pool_;
    };
}
#endif
