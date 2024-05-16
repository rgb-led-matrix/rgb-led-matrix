#ifndef OUTDOOR_H
#define OUTDOOR_H

#include <Mapper/Mapper.h>

namespace rgb_matrix::Mappers {
    class Outdoor : public Mapper {
        public:
            Color_Order map_color(uint16_t x, uint16_t y);
            cord_t map_location(uint16_t x, uint16_t y, cord_t size, uint8_t scan);
    };
}
#endif
