#include <assert.h>
#include "framebuffer/BCM/BCM.h"
#include "port/pin-mapper/HUB75/HUB75_Pins.h"

namespace rgb_matrix {
    extern struct PinMapping *hardware_mapping_;

    template <typename T> BCM<T>::BCM(DOTCorrect dot, GAMMA gamma) 
        : Framebuffer<T>(dot, gamma) {
            io = new GPIO();
            InitGPIO();
            *pin_mappings = HUB75_pin_mappings;
    }
    
    template <typename T> void BCM<T>::DumpToMatrix() {
        // TODO: Send buffer to GPIO
        //  Create thread class in port API
    }

    template <typename T> inline void  BCM<T>::MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) {
        // TODO: RGB Gamma, Brightness, Dot correction, CIE1931, etc.
    }

    template <typename T> void BCM<T>::InitGPIO() {
        const struct HUB75_Pins &h =  *((struct HUB75_Pins *) hardware_mapping_);
        gpio_bits_t all_used_bits = 0;
        all_used_bits |= h.TX | h.RX | h.reset | h.bootloader | h.cs_out |
            h.address0 | h.address1 | h.address2 | h.address3;

        const gpio_bits_t result = io->InitOutputs(all_used_bits);
        assert(result == all_used_bits); 
    }

    template class BCM<PixelDesignator>;
}