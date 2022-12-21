#include <pthread.h>
#include "port/thread/Thread.h"

namespace rgb_matrix {
    Thread::Thread() {
        thread = nullptr;
    }

    Thread::~Thread() {
        WaitStopped();
        delete reinterpret_cast<pthread_t *>(thread);
    }

    void Thread::WaitStopped() {
        if (thread != nullptr)
            pthread_join(*(reinterpret_cast<pthread_t *>(thread)), NULL);
    }
                
    bool Thread::Start(int priority, uint32_t affinity_mask) {
        if (thread == nullptr) {
            thread = new pthread_t;
            pthread_create(reinterpret_cast<pthread_t *>(thread), NULL, &func, this);

            if (priority > 0) {
                struct sched_param p;

                p.sched_priority = priority;
                pthread_setschedparam(*(reinterpret_cast<pthread_t *>(thread)), SCHED_FIFO, &p);
            }

            if (affinity_mask != 0) {
                cpu_set_t mask;
                CPU_ZERO(&mask);

                for (uint32_t i = 0; i < 32; i++) {
                    if ((affinity_mask & (1 << i)) != 0)
                        CPU_SET(i, &mask);
                }
                    
                pthread_setaffinity_np(*(reinterpret_cast<pthread_t *>(thread)), sizeof(mask), &mask);
            }
        }
        else
            return false;
        return true;
    }

    void *Thread::func(void *arg) {
        reinterpret_cast<Thread *>(arg)->Run();
        return NULL;
    }
}