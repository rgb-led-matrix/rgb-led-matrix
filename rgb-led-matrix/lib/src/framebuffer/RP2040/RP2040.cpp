#include <assert.h>
#include "framebuffer/RP2040/RP2040.h"
#include "port/pin-mapper/RP2040/RP2040_Pins.h"

namespace rgb_matrix {
    extern struct PinMapping *hardware_mapping_;

    template <typename T> RP2040<T>::RP2040(DOTCorrect dot, GAMMA gamma) 
        : Framebuffer<T>(dot, gamma) {
            io = new GPIO();
            InitGPIO();
            *pin_mappings = RP2040_pin_mappings;
    }
    
    template <typename T> void RP2040<T>::DumpToMatrix() {
        // TODO: Send buffer to RP2040 external hardware module
    }

    template <typename T> inline void  RP2040<T>::MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) {
        // TODO: RGB Gamma, Brightness, Dot correction, CIE1931, etc.
    }

    template <typename T> void RP2040<T>::InitGPIO() {
        const struct RP2040_Pins &h =  *((struct RP2040_Pins *) hardware_mapping_);
        gpio_bits_t all_used_bits = 0;
        all_used_bits |= h.TX | h.RX | h.reset | h.bootloader | h.cs_out |
            h.address0 | h.address1 | h.address2 | h.address3;

        const gpio_bits_t result = io->InitOutputs(all_used_bits);
        assert(result == all_used_bits); 
    }

    template class RP2040<PixelDesignator>;
}