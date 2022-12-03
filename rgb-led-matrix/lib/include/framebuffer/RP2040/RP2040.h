#ifndef RP2040_H
#define RP2040_H

#include "framebuffer/framebuffer.h"
#include "port/gpio/gpio.h"

namespace rgb_matrix {
class RP2040 : public Framebuffer {
    public:
        RP2040(int rows, int columns);

        virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
        virtual void Serialize(const char **data, size_t *len, Canvas_ID *id);
        virtual bool Deserialize(const char *data, size_t len, Canvas_ID id);
        virtual void DumpToMatrix();

    protected:
        virtual void InitGPIO();
        virtual inline gpio_bits_t *ValueAt(int double_row, int column, int bit);
        virtual inline void  MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue);

        GPIO io;
};
}
#endif