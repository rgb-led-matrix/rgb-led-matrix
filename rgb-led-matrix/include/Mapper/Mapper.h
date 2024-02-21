#ifndef MAPPER_H
#define MAPPER_H

#include <stdint.h>
#include <CFG/Types/types.h>

namespace rgb_matrix {
    // Pure interface for mapping
    class Mapper {
        public:
            virtual Color_Order map_color(uint16_t x, uint16_t y) = 0;
            virtual cord_t map_location(uint16_t x, uint16_t y, cord_t size, uint8_t scan) = 0;
    };
}
#endif
