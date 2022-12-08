#include "pin-pulser/PICO/PICO.h"

namespace rgb_matrix {
    PICO::PICO(GPIO *io, HUB75_Pins *pins) : PinPulser(io, pins) {
        io->ClearBits(pins->flag);
    }

    void PICO::trigger() {
        io->SetBits(pins->flag);
    }

    void PICO::wait() {
        io->ClearBits(pins->flag);
        while (!(io->Read() & pins->interrupt));
    }
}