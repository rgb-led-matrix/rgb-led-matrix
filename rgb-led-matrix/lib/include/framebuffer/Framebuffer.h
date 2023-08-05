#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include "PixelDesignatorMap.h"
#include "CFG/CFG.h"

namespace rgb_matrix {
  template <typename T> class Framebuffer {
    public:
      Framebuffer(CFG *cfg);
      virtual ~Framebuffer() {}

      static Framebuffer *CreateFramebuffer(CFG *cfg);

      virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
      virtual void show() = 0;

    protected:
      Framebuffer();

      virtual void  MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) = 0;

      CFG *cfg_;
      PixelDesignatorMap<T> *shared_mapper_;
  };
}
#endif
