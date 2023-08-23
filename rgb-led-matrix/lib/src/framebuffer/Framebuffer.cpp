#include <algorithm>
#include <assert.h>
#include <math.h>
#include "framebuffer/external/RP2040/RP2040_UART/RP2040_UART.h"
#include "framebuffer/RGB/RGB24.h"
#include "framebuffer/RGB/RGB48.h"
#include "framebuffer/RGB/RGB_555.h"
#include "framebuffer/RGB/RGB_222.h"
using std::min;
using std::max;

namespace rgb_matrix {
  // Don't use this!    
  template <typename T> Framebuffer<T>::Framebuffer() : cfg_(nullptr), buffer_(NULL) {
    assert(buffer_ != NULL);
  }

  template <typename T> Framebuffer<T>::Framebuffer(CFG *cfg) : cfg_(cfg) {
    assert(cfg != nullptr);

    build_table(cfg_->get_gamma());

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

  template <typename T> void Framebuffer<T>::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    T **ptr = (T **) buffer_;

    lock_.lock();
    if (x > 0 && x < cfg_->get_cols() && y > 0 && y < cfg_->get_rows())
      MapColors(x, y, red, green, blue, &ptr[x][y]);
    lock_.unlock();
  }

  // Handles brightness and gamma
  template <typename T> void Framebuffer<T>::build_table(GAMMA g) {
    for (uint32_t i = 0; i < 256; i++) {
      for (int j = 0; j < 100; j++) {
        lut[i][j].red = (uint8_t) round(pow(i / 255.0, 1 / g.get_red()) * T::red_lim * j / 99.0);
        lut[i][j].green = (uint8_t) round(pow(i / 255.0, 1 / g.get_green()) * T::green_lim * j / 99.0);
        lut[i][j].blue = (uint8_t) round(pow(i / 255.0, 1 / g.get_blue()) * T::blue_lim * j / 99.0);
      }
    }
  }

  template <> Framebuffer<RGB48> *Framebuffer<RGB48>::CreateFramebuffer(CFG *cfg) {
    switch (cfg->get_id()) {
      case External_ID::RP2040_UART_RGB48_ID:
        return new RP2040_UART<RGB48>(cfg);
      default:
        return nullptr;
    }
  }

  template <> Framebuffer<RGB24> *Framebuffer<RGB24>::CreateFramebuffer(CFG *cfg) {
    switch (cfg->get_id()) {
      case External_ID::RP2040_UART_RGB24_ID:
        return new RP2040_UART<RGB24>(cfg);
      default:
        return nullptr;
    }
  }

  template <> Framebuffer<RGB_555> *Framebuffer<RGB_555>::CreateFramebuffer(CFG *cfg) {
    switch (cfg->get_id()) {
      default:
        return nullptr;
    }
  }

  template <> Framebuffer<RGB_222> *Framebuffer<RGB_222>::CreateFramebuffer(CFG *cfg) {
    switch (cfg->get_id()) {
      default:
        return nullptr;
    }
  }

  template class Framebuffer<RGB48>;
  template class Framebuffer<RGB24>;
  template class Framebuffer<RGB_555>;
  template class Framebuffer<RGB_222>;
}  // namespace rgb_matrix
