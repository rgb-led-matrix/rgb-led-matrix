#include <math.h>
#include "framebuffer/external/RP2040/RP2040_UART/RP2040_UART.h"
#include "framebuffer/external/external.h"

namespace rgb_matrix {

  template <> Framebuffer<RGB24> *Framebuffer<RGB24>::CreateFramebuffer(CFG *cfg) {
    switch (cfg->get_id()) {
      case External_ID::RP2040_UART_RGB24_ID:
        return new RP2040_UART<RGB24>(cfg);
    }

    return nullptr;
  }

  template <> void Framebuffer<RGB24>::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    RGB24 **ptr = (RGB24 **) buffer_;

    if (x > 0 && x < cfg_->get_cols() && y > 0 && y < cfg_->get_rows())
      MapColors(x, y, red, green, blue, &ptr[x][y]);
  }

  // Handles brightness, gamma and CIE1931
  template <> void Framebuffer<RGB24>::build_table(GAMMA g, bool use_CIE1931) {
    if (!use_CIE1931) {
      for (uint32_t i = 0; i < 256; i++) {
        for (int j = 0; j < 100; j++) {
          constexpr uint32_t lim = 255;
          lut[i][j].red = (uint8_t) round(pow(i / 255.0, 1 / g.get_red()) * lim * j / 99.0);
          lut[i][j].green = (uint8_t) round(pow(i / 255.0, 1 / g.get_green()) * lim * j / 99.0);
          lut[i][j].blue = (uint8_t) round(pow(i / 255.0, 1 / g.get_blue()) * lim * j / 99.0);
        }
      }
    }
    else {
      for (uint32_t i = 0; i < 256; i++) {
        for (int j = 0; j < 100; j++) {
          constexpr uint32_t lim = 255;
          float temp = pow(i / 255.0, 1 / g.get_red()) * j;
          lut[i][j].red = (uint8_t) round(lim * ((temp <= 8) ? temp / 902.3 : pow((temp + 16) / 116.0, 3)));
          temp = pow(i / 255.0, 1 / g.get_green()) * j;
          lut[i][j].green = (uint8_t) round(lim * ((temp <= 8) ? temp / 902.3 : pow((temp + 16) / 116.0, 3)));
          temp = pow(i / 255.0, 1 / g.get_blue()) * j;
          lut[i][j].blue = (uint8_t) round(lim * ((temp <= 8) ? temp / 902.3 : pow((temp + 16) / 116.0, 3)));
        }
      }
    }
  }

  template class Framebuffer<RGB24>;
}  // namespace rgb_matrix
