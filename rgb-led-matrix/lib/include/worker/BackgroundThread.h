#ifndef BACKGROUND_THREAD_H
#define BACKGROUND_THREAD_H

#include <stdint.h>
#include "port/thread/Thread.h"
#include "port/pin-mapper/PinMapping.h"
#include "CFG/CFG.h"

namespace rgb_matrix {
    class BackgroundThread {
        public:
            static void CreateThread(CFG *cfg, PinMapping *pins);
            static void DestroyThread();

        private:
            static Thread *ptr;
    };
}
#endif
