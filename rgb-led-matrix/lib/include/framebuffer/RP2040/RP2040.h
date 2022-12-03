#ifndef RP2040_H
#define RP2040_H

#include "framebuffer/framebuffer.h"
#include "port/gpio/gpio.h"

namespace rgb_matrix {
template <typename T> class RP2040 : public Framebuffer<T> {
    public:
        RP2040(DOTCorrect dot);

        virtual void DumpToMatrix();

    protected:
        virtual void InitGPIO();
        virtual inline void  MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue);

        GPIO *io;
};
}
#endif