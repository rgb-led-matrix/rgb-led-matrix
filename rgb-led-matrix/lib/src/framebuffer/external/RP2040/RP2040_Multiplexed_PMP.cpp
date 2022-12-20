#include <assert.h>
#include "framebuffer/external/RP2040/RP2040_Multiplexed_PMP.h"
#include "port/pin-mapper/external/RP2040/RP2040_Multiplexed_PMP_Pins.h"

namespace rgb_matrix {
    template <typename T> RP2040_Multiplexed_PMP<T>::RP2040_Multiplexed_PMP(CFG *cfg) 
        : Framebuffer<T>(cfg) {
            io = new GPIO();
            InitGPIO();

            if (pin_mappings != nullptr)
                throw pin_mappings;

            pin_mappings = RP2040_Multiplexed_PMP_pin_mappings;
            pin_mappings_size = RP2040_Multiplexed_PMP_pin_mappings_size;
    }
    
    template <typename T> void RP2040_Multiplexed_PMP<T>::DumpToMatrix() {
        // TODO: Send buffer to RP2040 external hardware module
    }

    template <typename T> inline void  RP2040_Multiplexed_PMP<T>::MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) {
        // TODO: RGB Gamma, Brightness, Dot correction, CIE1931, etc.
    }

    template <typename T> void RP2040_Multiplexed_PMP<T>::InitGPIO() {
        const struct RP2040_Multiplexed_PMP_Pins h =  RP2040_Multiplexed_PMP_pin_mappings[Framebuffer<T>::hardware_mapping_];
        gpio_bits_t all_used_bits = h.wr.wr[0] | h.wr.wr[1] | h.wr.wr[2] | h.wr.wr[3] | h.reset;

        for (gpio_bits_t i = 0; i < 8; i++)
            all_used_bits |= 1 << (h.dat + i);

        const gpio_bits_t result = io->InitOutputs(all_used_bits);
        assert(result == all_used_bits); 
    }

    template class RP2040_Multiplexed_PMP<PixelDesignator>;
}