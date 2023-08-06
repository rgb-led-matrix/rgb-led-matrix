#include <math.h>
#include "framebuffer/external/RP2040/RP2040_UART/RP2040_UART.h"
#include "framebuffer/RGB/RGB24.h"

namespace rgb_matrix {
    // Handles dot correction and PWM bit scaling
    template <> inline void  RP2040_UART<RGB24>::MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, RGB24 *pixel) {
        float fr, fg, fb;
        uint8_t bright =  brightness_;

        cfg_->get_dot().get(x, y, r, g, b, &fr, &fg, &fb);
        pixel->red = (uint8_t) round(lut[r][bright].red / 255.0 * fr * cfg_->get_pwm_bits());
        pixel->green = (uint8_t) round(lut[g][bright].green / 255.0 * fg * cfg_->get_pwm_bits());
        pixel->blue = (uint8_t) round(lut[b][bright].blue / 255.0 * fb * cfg_->get_pwm_bits());
    }

    template class RP2040_UART<RGB24>;
}