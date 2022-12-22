#include <assert.h>
#include <cmath>
#include "framebuffer/external/RP2040/RP2040_SPI.h"

namespace rgb_matrix {
    template <typename T> RP2040_SPI<T>::RP2040_SPI(CFG *cfg) 
        : Framebuffer<T>(cfg) {
            if (cfg->get_id() == Canvas_ID::RP2040_SPI_ID)
                cfg_ = static_cast<RP2040_SPI_CFG *>(cfg);
            else
                throw cfg;

            if (cfg_->use_gamma_correction)
                lut = std::vector<std::vector<table>>(cfg->dot.rows, std::vector<table>(cfg->dot.cols, table(cfg->gamma, cfg_->use_CIE1931)));
            else
                lut = std::vector<std::vector<table>>(cfg->dot.rows, std::vector<table>(cfg->dot.cols, table(GAMMA(1.0, 1.0, 1.0), cfg_->use_CIE1931)));
    }
    
    template <typename T> void RP2040_SPI<T>::DumpToMatrix() {
        // TODO: Send buffer to RP2040 external hardware module
    }

    template <typename T> inline void  RP2040_SPI<T>::MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) {
        float fr, fg, fb;
        uint8_t bright = cfg_->use_brightness ? cfg_->brightness_ % 100 : 100;
        cfg_->dot.get(x, y, r, g, b, &fr, &fg, &fb);
        *red = (uint16_t) round(lut[x][y].val[r][bright][0] / 65535.0 * fr * cfg_->pwm_bits_);
        *green = (uint16_t) round(lut[x][y].val[g][bright][1] / 65535.0 * fg * cfg_->pwm_bits_);
        *blue = (uint16_t) round(lut[x][y].val[b][bright][2] / 65535.0 * fb * cfg_->pwm_bits_);
    }

    template <typename T> RP2040_SPI<T>::table::table(GAMMA g, bool use_CIE1931) {
        if (!use_CIE1931) {
            for (uint32_t i = 0; i < 256; i++) {
                for (int j = 0; j < 100; j++) {
                    val[i][j][0] = (int) round(pow(i * 65535 / 255 * round(j / 99.0), 1 / g.red));
                    val[i][j][1] = (int) round(pow(i * 65535 / 255 * round(j / 99.0), 1 / g.green));
                    val[i][j][2] = (int) round(pow(i * 65535 / 255 * round(j / 99.0), 1 / g.blue));
                }
            }
        }
        else {
            // TODO:
        }
    }

    template class RP2040_SPI<PixelDesignator>;
}