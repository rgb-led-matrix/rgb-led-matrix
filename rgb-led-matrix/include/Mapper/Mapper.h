#ifndef MAPPER_H
#define MAPPER_H

#include <stdint.h>
#include <CFG/Types/types.h>

namespace rgb_matrix {
    // Pure interface for drawing
    class Mapper {
        public:
            virtual void SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue) = 0;
            virtual void SetPixel(cord_t cord, pixel_t pixel) { SetPixel(cord.x, cord.y, pixel.red, pixel.green, pixel.blue); }
            virtual cord_t get_size() = 0;
            virtual void set_brightness(uint8_t brightness) = 0;
            virtual void map_wavelength(uint8_t color, Color index, uint16_t value) = 0;
    };
}
#endif
