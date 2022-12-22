#include <assert.h>
#include "framebuffer/external/RP2040/RP2040_SPI.h"

namespace rgb_matrix {
    template <typename T> RP2040_SPI<T>::RP2040_SPI(CFG *cfg) 
        : Framebuffer<T>(cfg) {
            // TODO:
    }
    
    template <typename T> void RP2040_SPI<T>::DumpToMatrix() {
        // TODO: Send buffer to RP2040 external hardware module
    }

    template <typename T> inline void  RP2040_SPI<T>::MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) {
        // TODO: RGB Gamma, Brightness, Dot correction, CIE1931, etc.
    }

    template class RP2040_SPI<PixelDesignator>;
}