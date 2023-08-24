#ifndef FRAMEBUFFER_RGB_555_H
#define FRAMEBUFFER_RGB_555_H

#include <stdint.h>

namespace rgb_matrix {
    struct RGB_555 {
        RGB_555() : red(0), green(0), blue(0) {}

        uint16_t red : 5;
        uint16_t green : 5;
        uint16_t blue : 5;

        static constexpr uint32_t red_lim = 5;
        static constexpr uint32_t green_lim = 5;
        static constexpr uint32_t blue_lim = 5;

        static constexpr float red_max = 31.0;
        static constexpr float green_max = 31.0;
        static constexpr float blue_max = 31.0;
    };
}

#endif