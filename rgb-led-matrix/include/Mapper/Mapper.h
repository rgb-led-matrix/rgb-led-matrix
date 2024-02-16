#ifndef MAPPER_H
#define MAPPER_H

#include <stdint.h>
#include <CFG/types.h>

namespace rgb_matrix {
    class Mapper {
        public:
            virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) = 0;
            virtual void SetPixel(cord_t cord, pixel_t pixel) { SetPixel(cord.x, cord.y, pixel.red, pixel.green, pixel.blue); }
            virtual cord_t get_size() = 0;
    };
}
#endif
