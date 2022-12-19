#include <chrono>
#include <thread>
#include "pin-pulser/PICO/PICO.h"

namespace rgb_matrix {
    PICO::PICO(GPIO *io, HUB75_Pins *pins) : PinPulser(io, pins) {
        io->ClearBits(pins->flag);
        io->SetBits(pins->reset);
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        io->ClearBits(pins->reset);
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    void PICO::trigger() {
        io->SetBits(pins->flag);
    }

    void PICO::wait() {
        io->ClearBits(pins->flag);
        while (!(io->Read() & pins->interrupt));
    }
}