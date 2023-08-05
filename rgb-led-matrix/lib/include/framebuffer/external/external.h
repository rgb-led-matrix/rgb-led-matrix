#ifndef EXTERNAL_FRAMEBUFFER_H
#define EXTERNAL_FRAMEBUFFER_H

#include <stdint.h>

namespace rgb_matrix {
    struct RGB48 {
        RGB48() : red(0), green(0), blue(0) {}

        uint16_t red;
        uint16_t green;
        uint16_t blue;
    };
}

#endif