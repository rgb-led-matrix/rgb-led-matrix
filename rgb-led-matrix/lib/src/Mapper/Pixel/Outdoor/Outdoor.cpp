#include <Mapper/Pixel/Outdoor/Outdoor.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    // Do not use this!    
    Outdoor::Outdoor() : Pixel() {
        throw Illegal("Panel Pixel Mapper");
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
        // TODO:
        cord_t result;
        result.x = 0;
        result.y = 0;
        return result;
    }

    Color_Order Outdoor::map_color(uint16_t x, uint16_t y) {
        // TODO:

        return Color_Order::RGB;
    }
}
