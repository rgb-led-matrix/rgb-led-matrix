#include <algorithm>
#include <assert.h>
#include <stdio.h>
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
    //buffer_ = new T[cfg->get_cols()][cfg->get_rows()];
    brightness_ = 100;
  }

  template <typename T> Framebuffer<T> *Framebuffer<T>::CreateFramebuffer(CFG *cfg) {
    switch (cfg->get_id()) {
      case Canvas_ID::RP2040_UART_ID:
        return new RP2040_UART<T>(cfg);
    }

    return nullptr;
  }

  template <> void Framebuffer<RGB48>::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    RGB48 **ptr = (RGB48 **) buffer_;

    if (x > 0 && x < cfg_->get_cols() && y > 0 && y < cfg_->get_rows()) {
      RGB48 pixel = ptr[x][y];
      MapColors(x, y, red, green, blue, &pixel.red, &pixel.green, &pixel.blue);
    }
  }

  template <typename T> void Framebuffer<T>::set_brightness(uint8_t brightness) {
    brightness_ = max(min(brightness, (uint8_t) 100), (uint8_t) 0);
  }

  template class Framebuffer<RGB48>;
}  // namespace rgb_matrix
