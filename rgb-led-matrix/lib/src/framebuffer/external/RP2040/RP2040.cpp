#include <math.h>
#include "framebuffer/external/RP2040/RP2040.h"
#include "framebuffer/RGB/RGB24.h"
#include "framebuffer/RGB/RGB48.h"
#include "framebuffer/RGB/RGB_222.h"
#include "framebuffer/RGB/RGB_555.h"

namespace rgb_matrix {
    template <typename T> RP2040<T>::RP2040(CFG *cfg) : Framebuffer<T>(cfg) {            
        
    }

    // Handles dot correction and PWM bit scaling
    template <typename T> inline void  RP2040<T>::MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, T *pixel) {
        float fr, fg, fb;
        uint8_t bright =  this->brightness_;

        cfg_->get_dot().get(x, y, r, g, b, &fr, &fg, &fb);
        pixel->red = (uint16_t) round(this->lut[bright][r].red / T::red_max * fr * cfg_->get_pwm_bits());
        pixel->green = (uint16_t) round(this->lut[bright][g].green / T::green_max * fg * cfg_->get_pwm_bits());
        pixel->blue = (uint16_t) round(this->lut[bright][b].blue / T::blue_max * fb * cfg_->get_pwm_bits());
    }

    template class RP2040<RGB24>;
    template class RP2040<RGB48>;
    template class RP2040<RGB_222>;
    template class RP2040<RGB_555>;
}