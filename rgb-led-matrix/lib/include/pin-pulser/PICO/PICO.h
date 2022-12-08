#ifndef PICO_H
#define PICO_H

#include "pin-pulser/PinPulser.h"

namespace rgb_matrix {
    class PICO : public PinPulser {
        public:
            PICO(GPIO *io, HUB75_Pins *pins);

            void trigger();
            void wait();
    };
}
#endif