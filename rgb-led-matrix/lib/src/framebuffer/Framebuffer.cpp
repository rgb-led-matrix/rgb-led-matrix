#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "framebuffer/external/RP2040/RP2040_UART/RP2040_UART.h"
#include "framebuffer/external/external.h"
using std::min;
using std::max;

namespace rgb_matrix {
  // Don't use this!    
  template <typename T> Framebuffer<T>::Framebuffer() : cfg_(nullptr), buffer_(NULL) {
    assert(buffer_ != NULL);
  }

  template <typename T> Framebuffer<T>::Framebuffer(CFG *cfg) : cfg_(cfg) {
    assert(cfg != nullptr);

    for (int i = 0; i < cfg->get_cols(); i++)
      buffer_[i] = new T[cfg->get_rows()];
      
    brightness_ = 100;
  }

  template <typename T> Framebuffer<T> *Framebuffer<T>::CreateFramebuffer(CFG *cfg) {
    switch (cfg->get_id()) {
      case Panel_ID::RP2040_UART_RGB48_ID:
        return new RP2040_UART<T>(cfg);
    }

    return nullptr;
  }

  template <> void Framebuffer<RGB48>::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    RGB48 **ptr = (RGB48 **) buffer_;

    if (x > 0 && x < cfg_->get_cols() && y > 0 && y < cfg_->get_rows())
      MapColors(x, y, red, green, blue, &ptr[x][y]);
  }

  template <typename T> void Framebuffer<T>::set_brightness(uint8_t brightness) {
    brightness_ = max(min(brightness, (uint8_t) 100), (uint8_t) 0);
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
