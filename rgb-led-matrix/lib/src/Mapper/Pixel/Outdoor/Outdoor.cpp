#include <Mapper/Pixel/Outdoor/Outdoor.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    // Do not use this!    
    Outdoor::Outdoor() : Pixel() {
        throw Illegal("Outdoor Pixel Mapper");
    }

    Outdoor::Outdoor(Single_Panel *panel) : Pixel(panel) {
        map();
    }

    cord_t Outdoor::get_actual_size() {
        cord_t result;
        result.x = size_.x * 2;
        result.y = size_.y / 2;
        return result;
    }


    cord_t Outdoor::map_location(uint16_t x, uint16_t y) {
        cord_t result;
        result.x = (x * y) % (size_.x * 2);
        result.y = (x * y) / (size_.x * 2);
        return result;
    }

    Color_Order Outdoor::map_color(uint16_t x, uint16_t y) {
        return Color_Order::RGB;
    }
}
