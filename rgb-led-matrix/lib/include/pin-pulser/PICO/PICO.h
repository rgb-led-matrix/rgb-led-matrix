#ifndef PICO_H
#define PICO_H

#include "pin-pulser/PinPulser.h"

// Note: This supports reconfiguration via USB bootloader using helper to reset PICO.
//  The reset used in this logic is used sync state. (There is a soft and a hard reset.)

namespace rgb_matrix {
    class PICO : public PinPulser {
        public:
            PICO(GPIO *io, HUB75_Pins *pins);

            void trigger();
            void wait();
    };
}
#endif