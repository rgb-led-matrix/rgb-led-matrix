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

  template <typename T> Framebuffer<T>::~Framebuffer() {
    for (int i = 0; i < cfg_->get_cols(); i++)
      delete buffer_[i];
  }

  template <typename T> void Framebuffer<T>::set_brightness(uint8_t brightness) {
    lock_.lock();
    brightness_ = max(min(brightness, (uint8_t) 100), (uint8_t) 0);
    lock_.unlock();
  }

  template <typename T> cord_t Framebuffer<T>::get_size() {
    cord_t result;

    result.x = cfg_->get_cols();
    result.y = cfg_->get_rows();

    return result;
  }

  template<typename T> void Framebuffer<T>::show() {
    lock_.lock();
    show_internal();
    lock_.unlock();
  }
}  // namespace rgb_matrix
