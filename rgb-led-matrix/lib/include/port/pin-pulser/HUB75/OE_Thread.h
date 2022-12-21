#ifndef OE_THREAD_H
#define OE_THREAD_H

#include "port/thread/Thread.h"
#include "port/gpio/gpio.h"
#include "CFG/CFG.h"
#include "port/pin-mapper/HUB75/HUB75_Pins.h"

namespace rgb_matrix {
    template <typename T> class OE_Thread : public Thread {
        public:
            OE_Thread(CFG *cfg, HUB75_Pins *pins);
            virtual ~OE_Thread() {}

            static Thread *CreateThread(CFG *cfg, HUB75_Pins *pins);

        protected:
            void Run();

            GPIO *io_;
            HUB75_Pins *pins_;
    };
}

#endif