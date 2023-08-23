#ifndef FRAMEBUFFER_RGB24_H
#define FRAMEBUFFER_RGB24_H

#include <stdint.h>

namespace rgb_matrix {
    struct RGB24 {
        RGB24() : red(0), green(0), blue(0) {}

        uint8_t red;
        uint8_t green;
        uint8_t blue;

        static constexpr uint32_t red_lim = 8;
        static constexpr uint32_t green_lim = 8;
        static constexpr uint32_t blue_lim = 8;
    };
}

#endif