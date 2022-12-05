#ifndef EXTERNAL_H
#define EXTERNAL_H

#include "stdint.h"

namespace rgb_matrix {
    struct PixelDesignator {
        PixelDesignator() : r_bit(0), g_bit(0), b_bit(0) {}

        uint16_t r_bit;
        uint16_t g_bit;
        uint16_t b_bit;
    };
}

#endif