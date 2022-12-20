#ifndef BCM_THREAD_H
#define BCM_THREAD_H

#include "port/thread/Thread.h"
#include "CFG/CFG.h"

namespace rgb_matrix {
    class BCM_Thread : public Thread {
        public:
            BCM_Thread(CFG *cfg);
            ~BCM_Thread();

        protected:
            void Run();
    };
}

#endif