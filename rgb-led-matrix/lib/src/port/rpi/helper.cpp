#include <chrono>
#include <thread>
#include <string>
#include "port/gpio/gpio.h"
using namespace rgb_matrix;

int main(int argc, char **argv) {
    if (argc >= 2) {
        GPIO *io = GPIO::getGPIO();
        gpio_bits_t pin = 1 << std::stoi(argv[1]);

        io->Init();
        io->InitOutputs(pin);
        io->ClearBits(pin);
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        io->SetBits(pin);
        std::this_thread::sleep_for(std::chrono::seconds(10));

        // TODO: Release and make input?
    }

    return 0;
}
