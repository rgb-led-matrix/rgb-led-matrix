#ifndef FRAMEBUFFER_RGB_232_H
#define FRAMEBUFFER_RGB_232_H

#include <stdint.h>

namespace rgb_matrix {
    struct RGB_232 {
        RGB_232() : red(0), green(0), blue(0) {}

        uint8_t red : 2;
        uint8_t green : 3;
        uint8_t blue : 2;
    };
}

#endif