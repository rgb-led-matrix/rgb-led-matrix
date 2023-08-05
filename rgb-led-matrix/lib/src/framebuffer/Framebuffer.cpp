#include <assert.h>
#include <stdio.h>
#include "framebuffer/external/RP2040/RP2040_UART/RP2040_UART.h"
#include "framebuffer/external/external.h"

namespace rgb_matrix {
  // Don't use this!    
  template <typename T> Framebuffer<T>::Framebuffer()
    : cfg_(nullptr), shared_mapper_(NULL) {
    assert(shared_mapper_ != NULL);
  }

  template <typename T> Framebuffer<T>::Framebuffer(CFG *cfg)
    : cfg_(cfg) {
    assert(cfg != nullptr);

    shared_mapper_ = new PixelDesignatorMap<T>(cfg->get_dot().cols, cfg->get_dot().rows);
  }

  template <> Framebuffer<PixelDesignator> *Framebuffer<PixelDesignator>::CreateFramebuffer(CFG *cfg) {
    switch (cfg->get_id()) {
      case Canvas_ID::RP2040_UART_ID:
        return new RP2040_UART<PixelDesignator>(cfg);
    }

    return nullptr;
  }

  // TODO: Fix this
  template <> void Framebuffer<PixelDesignator>::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    PixelDesignator *pixel = shared_mapper_->get(*shared_mapper_->get(x, y));
    MapColors(x, y, red, green, blue, &pixel->r_bit, &pixel->g_bit, &pixel->b_bit);
  }

  template class Framebuffer<PixelDesignator>;

}  // namespace rgb_matrix
