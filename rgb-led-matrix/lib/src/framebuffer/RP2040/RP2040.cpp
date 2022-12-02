#include "port/pin-mapper/RP2040/RP2040_Pins.h"
#include "framebuffer/RP2040/RP2040.h"

namespace rgb_matrix {
    RP2040::RP2040(int rows, int columns, PixelDesignatorMap **mapper) 
        : Framebuffer(rows, columns, mapper) {

    }
}