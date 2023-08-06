#include <math.h>
#include "framebuffer/external/external.h"
#include "framebuffer/external/RP2040/RP2040_UART/RP2040_UART.h"

namespace rgb_matrix {
    // Handles dot correction and PWM bit scaling
    template <> inline void  RP2040_UART<RGB48>::MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, RGB48 *pixel) {
        float fr, fg, fb;
        uint8_t bright =  brightness_;

        cfg_->get_dot().get(x, y, r, g, b, &fr, &fg, &fb);
        pixel->red = (uint16_t) round(lut[r][bright].red / 65535.0 * fr * cfg_->get_pwm_bits());
        pixel->green = (uint16_t) round(lut[g][bright].green / 65535.0 * fg * cfg_->get_pwm_bits());
        pixel->blue = (uint16_t) round(lut[b][bright].blue / 65535.0 * fb * cfg_->get_pwm_bits());
    }

    template class RP2040_UART<RGB48>;
}