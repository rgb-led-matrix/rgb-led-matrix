#ifndef FRAMEBUFFER_RGB24_H
#define FRAMEBUFFER_RGB24_H

#include <stdint.h>

namespace rgb_matrix {
    struct RGB24 {
        RGB24() : red(0), green(0), blue(0) {}

        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };
}

#endif