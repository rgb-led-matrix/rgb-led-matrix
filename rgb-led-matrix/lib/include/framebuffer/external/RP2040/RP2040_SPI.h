#ifndef RP2040_H
#define RP2040_H

#include "framebuffer/framebuffer.h"

namespace rgb_matrix {
    template <typename T> class RP2040_SPI: public Framebuffer<T> {
        public:
            RP2040_SPI(CFG *cfg);
            ~RP2040_SPI() {}

            virtual void DumpToMatrix();

        protected:
            virtual void  MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue);
    };
}

#endif