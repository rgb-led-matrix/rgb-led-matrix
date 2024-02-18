#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

namespace rgb_matrix {
    struct cord_t {
        uint16_t x;
        uint16_t y;
    };

    enum Color_Order {
        RGB = 0,
        RBG = 1,
        BRG = 2,
        BGR = 3,
        GRB = 4,
        GBR = 5,
    };

    enum Color {
        Red = 0,
        Green = 1,
        Blue = 2,
    };

    struct pixel_t {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    enum Data_Format_ID {
        RGB48_ID = 0,
        RGB24_ID = 1,
        RGB_222_ID = 2,
        RGB_555_ID = 3,
    };
}
#endif