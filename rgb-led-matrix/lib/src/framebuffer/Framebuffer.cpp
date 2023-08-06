#include <algorithm>
#include <assert.h>
#include "framebuffer/external/RP2040/RP2040_UART/RP2040_UART.h"
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

  template <typename T> void Framebuffer<T>::set_brightness(uint8_t brightness) {
    brightness_ = max(min(brightness, (uint8_t) 100), (uint8_t) 0);
  }
}  // namespace rgb_matrix
