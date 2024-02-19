#include <ThreadPool/ThreadPool.h>
#include <Panel/MultiPanel_Internal.h>

namespace rgb_matrix {
    template <typename R, typename... F> void ThreadPool<R, F...>::start(uint8_t count) {
        if (count == 0) {
            count = std::thread::hardware_concurrency();

            if (count == 0)
                count = 1;
        }

        for (uint8_t i = 0; i < count; i++) {
            threads.emplace_back(std::thread(&ThreadPool::ThreadLoop,this));
        }
    }

    template <typename R, typename... F> void ThreadPool<R, F...>::stop() {
        // TODO:
    }

    template <typename R, typename... F> bool ThreadPool<R, F...>::busy() {
        // TODO:
        return false;
    }

    template <typename R, typename... F> void ThreadPool<R, F...>::submit(const std::function<void(R,F...)>& job, R, F...) {
        // TODO:
    }

    template <typename R, typename... F> void ThreadPool<R, F...>::ThreadLoop(ThreadPool<R, F...> *object) {
        // TODO:
    }

    template class ThreadPool<void *, MultiPanel_Internal *, MultiPanel_Internal::Panel_t *>;
    template class ThreadPool<void *, MultiPanel_Internal::Panel_t *, MultiPanel_Internal *, uint8_t, Color, uint16_t>;
    template class ThreadPool<void *, MultiPanel_Internal::Panel_t *, MultiPanel_Internal *, uint8_t>;
    template class ThreadPool<void *, void *>;
}