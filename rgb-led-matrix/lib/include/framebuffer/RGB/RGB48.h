#ifndef FRAMEBUFFER_RGB48_H
#define FRAMEBUFFER_RGB48_H

#include <stdint.h>

namespace rgb_matrix {
    struct RGB48 {
        RGB48() : red(0), green(0), blue(0) {}

        uint16_t red;
        uint16_t green;
        uint16_t blue;

        static constexpr uint32_t red_lim = 16;
        static constexpr uint32_t green_lim = 16;
        static constexpr uint32_t blue_lim = 16;
    };
}

#endif