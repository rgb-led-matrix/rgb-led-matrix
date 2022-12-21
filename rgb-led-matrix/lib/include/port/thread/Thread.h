#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>

namespace rgb_matrix {
    class Thread {
        public:
            Thread();
            virtual ~Thread();

            void WaitStopped();
            bool Start(int realtime_priority = 0, uint32_t cpu_affinity_mask = 0);

        protected:
            virtual void Run() = 0;

        private:
            static void *func(void *arg);

            void *thread;
    };
}
#endif
