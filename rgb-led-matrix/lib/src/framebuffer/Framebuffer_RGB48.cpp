#include <math.h>
#include "framebuffer/external/RP2040/RP2040_UART/RP2040_UART.h"
#include "framebuffer/RGB/RGB48.h"

namespace rgb_matrix {
  template <> Framebuffer<RGB48> *Framebuffer<RGB48>::CreateFramebuffer(CFG *cfg) {
    switch (cfg->get_id()) {
      case External_ID::RP2040_UART_RGB48_ID:
        return new RP2040_UART<RGB48>(cfg);
      default:
        return nullptr;
    }
  }

  template <> void Framebuffer<RGB48>::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    RGB48 **ptr = (RGB48 **) buffer_;

    lock_.lock();
    if (x > 0 && x < cfg_->get_cols() && y > 0 && y < cfg_->get_rows())
      MapColors(x, y, red, green, blue, &ptr[x][y]);
    lock_.unlock();
  }

  // Handles brightness, gamma and CIE1931
  template <> void Framebuffer<RGB48>::build_table(GAMMA g, bool use_CIE1931) {
    if (!use_CIE1931) {
      for (uint32_t i = 0; i < 256; i++) {
        for (int j = 0; j < 100; j++) {
          constexpr uint32_t lim = 65535;
          lut[i][j].red = (uint16_t) round(pow(i / 255.0, 1 / g.get_red()) * lim * j / 99.0);
          lut[i][j].green = (uint16_t) round(pow(i / 255.0, 1 / g.get_green()) * lim * j / 99.0);
          lut[i][j].blue = (uint16_t) round(pow(i / 255.0, 1 / g.get_blue()) * lim * j / 99.0);
        }
      }
    }
    else {
      for (uint32_t i = 0; i < 256; i++) {
        for (int j = 0; j < 100; j++) {
          constexpr uint32_t lim = 65535;
          float temp = pow(i / 255.0, 1 / g.get_red()) * j;
          lut[i][j].red = (uint16_t) round(lim * ((temp <= 8) ? temp / 902.3 : pow((temp + 16) / 116.0, 3)));
          temp = pow(i / 255.0, 1 / g.get_green()) * j;
          lut[i][j].green = (uint16_t) round(lim * ((temp <= 8) ? temp / 902.3 : pow((temp + 16) / 116.0, 3)));
          temp = pow(i / 255.0, 1 / g.get_blue()) * j;
          lut[i][j].blue = (uint16_t) round(lim * ((temp <= 8) ? temp / 902.3 : pow((temp + 16) / 116.0, 3)));
        }
      }
    }
  }

  template class Framebuffer<RGB48>;
}  // namespace rgb_matrix
