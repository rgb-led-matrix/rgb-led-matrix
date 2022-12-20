#ifndef UPDATE_THREAD_H
#define UPDATE_THREAD_H

#include <stdint.h>
#include "port/thread/Thread.h"
#include "CFG/CFG.h"

namespace rgb_matrix {
    class UpdateThread {
        public:
            static void CreateThread(CFG *cfg);
            static void DestroyThread();

        private:
            static Thread *ptr;
    };
}
#endif
