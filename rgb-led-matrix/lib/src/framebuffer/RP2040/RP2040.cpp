#include <assert.h>
#include "framebuffer/RP2040/RP2040.h"
#include "port/pin-mapper/RP2040/RP2040_Pins.h"

namespace rgb_matrix {
    RP2040::RP2040(int rows, int columns) 
        : Framebuffer(rows, columns) {
            io = new GPIO();
            InitGPIO();
    }
    
    void RP2040::DumpToMatrix() {

    }

    inline void  RP2040::MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) {

    }

    void RP2040::InitGPIO() {
        const struct RP2040_Pins &h =  *((struct RP2040_Pins *) hardware_mapping_);
        gpio_bits_t all_used_bits = 0;
        all_used_bits |= h.TX | h.RX | h.reset | h.bootloader | h.cs_out |
            h.address0 | h.address1 | h.address2 | h.address3;

        const gpio_bits_t result = io->InitOutputs(all_used_bits);
        assert(result == all_used_bits); 
    }

}