#ifndef PIN_PULSER_H
#define PIN_PULSER_H

#include "port/gpio/gpio.h"
#include "port/pin-mapper/HUB75/HUB75_Pins.h"

namespace rgb_matrix {
    class PinPulser {
        public:
            PinPulser(GPIO *io, HUB75_Pins *pins);
            ~PinPulser() {}

            virtual void trigger() = 0;
            virtual void wait() = 0;
        
        protected:
            GPIO *io;
            HUB75_Pins *pins;
    };
}
#endif