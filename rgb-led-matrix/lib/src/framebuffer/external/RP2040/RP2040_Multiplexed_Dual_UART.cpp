#include <assert.h>
#include "framebuffer/external/RP2040/RP2040_Multiplexed_Dual_UART.h"
#include "port/pin-mapper/external/RP2040/RP2040_Multiplexed_Dual_UART_Pins.h"

namespace rgb_matrix {
    extern struct PinMapping *hardware_mapping_;

    template <typename T> RP2040_Multiplexed_Dual_UART<T>::RP2040_Multiplexed_Dual_UART(Canvas_ID id, CFG *cfg) 
        : Framebuffer<T>(id, cfg) {
            io = new GPIO();
            InitGPIO();
            *pin_mappings = RP2040_Multiplexed_Dual_UART_pin_mappings;
    }
    
    template <typename T> void RP2040_Multiplexed_Dual_UART<T>::DumpToMatrix() {
        // TODO: Send buffer to RP2040 external hardware module
    }

    template <typename T> inline void  RP2040_Multiplexed_Dual_UART<T>::MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) {
        // TODO: RGB Gamma, Brightness, Dot correction, CIE1931, etc.
    }

    template <typename T> void RP2040_Multiplexed_Dual_UART<T>::InitGPIO() {
        const struct RP2040_Multiplexed_Dual_UART_Pins &h =  *((struct RP2040_Multiplexed_Dual_UART_Pins *) hardware_mapping_);
        gpio_bits_t all_used_bits = h.cs.cs[0] | h.cs.cs[1] | h.cs.cs[2] | h.cs.cs[3];

        const gpio_bits_t result = io->InitOutputs(all_used_bits);
        assert(result == all_used_bits); 
    }

    template class RP2040_Multiplexed_Dual_UART<PixelDesignator>;
}