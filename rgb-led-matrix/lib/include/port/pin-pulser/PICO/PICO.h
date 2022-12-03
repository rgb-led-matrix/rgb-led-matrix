#ifndef PICO_H
#define PICO_H

#include "port/pin-pulser/PinPulser.h"

namespace rgb_matrix {
    class PICO : public PinPulser {
        public:
            PICO();
    };
}
#endif