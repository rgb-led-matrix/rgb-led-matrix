#ifndef HUB75_FRAMEBUFFER_H
#define HUB75_FRAMEBUFFER_H

#include "stdint.h"

namespace rgb_matrix {
    struct PixelDesignator_HUB75 {
        PixelDesignator_HUB75() : r_bit(0), g_bit(0), b_bit(0) {}

        uint16_t r_bit;
        uint16_t g_bit;
        uint16_t b_bit;
    };
}

#endif