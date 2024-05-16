#include <Mapper/Outdoor/Outdoor.h>

namespace rgb_matrix::Mappers {
    cord_t Outdoor::map_location(uint16_t x, uint16_t y, cord_t size, uint8_t scan) {
        cord_t result;
        result.x = (x * y) % (size.x * size.y / scan);
        result.y = (x * y) / (size.x * size.y / scan);
        return result;
    }

    Color_Order Outdoor::map_color(uint16_t x, uint16_t y) {
        return Color_Order::RGB;
    }
}
