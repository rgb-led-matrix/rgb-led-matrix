#ifndef RP2040_H
#define RP2040_H

#include "framebuffer/framebuffer.h"

namespace rgb_matrix {
class RP2040 : public Framebuffer {
    public:
        RP2040(int rows, int columns, PixelDesignatorMap **mapper);
};
}
#endif