#include <assert.h>
#include "framebuffer/external/RP2040/RP2040_Multiplexed_PMP.h"
#include "port/pin-mapper/external/RP2040/RP2040_Multiplexed_PMP_Pins.h"

namespace rgb_matrix {
    extern struct PinMapping *hardware_mapping_;

    template <typename T> RP2040_Multiplexed_PMP<T>::RP2040_Multiplexed_PMP(Canvas_ID id, CFG *cfg) 
        : Framebuffer<T>(id, cfg) {
            io = new GPIO();
            InitGPIO();
            *pin_mappings = RP2040_Multiplexed_PMP_pin_mappings;
    }
    
    template <typename T> void RP2040_Multiplexed_PMP<T>::DumpToMatrix() {
        // TODO: Send buffer to RP2040 external hardware module
    }

    template <typename T> inline void  RP2040_Multiplexed_PMP<T>::MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) {
        // TODO: RGB Gamma, Brightness, Dot correction, CIE1931, etc.
    }

    template <typename T> void RP2040_Multiplexed_PMP<T>::InitGPIO() {
        const struct RP2040_Multiplexed_PMP_Pins &h =  *((struct RP2040_Multiplexed_PMP_Pins *) hardware_mapping_);
        gpio_bits_t all_used_bits = h.cs.cs[0] | h.cs.cs[1] | h.cs.cs[2] | h.cs.cs[3] | h.clk;
        for (int i = 0; i < 8; i++)
            all_used_bits |= 1 << (h.dat + i);

        const gpio_bits_t result = io->InitOutputs(all_used_bits);
        assert(result == all_used_bits); 
    }

    template class RP2040_Multiplexed_PMP<PixelDesignator>;
}