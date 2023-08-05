#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include "CFG/CFG.h"
#include "Panel/Panel.h"

namespace rgb_matrix {
  template <typename T> class Framebuffer : public Panel {
    public:
      virtual ~Framebuffer() {}

      static Framebuffer *CreateFramebuffer(CFG *cfg);

      virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
      virtual void show() = 0;
      virtual void set_brightness(uint8_t brightness);

    protected:
      Framebuffer();
      Framebuffer(CFG *cfg);

      void build_table(GAMMA g, bool use_CIE1931);

      virtual void  MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, T *color) = 0;

      CFG *cfg_;
      T **buffer_;
      T lut[256][100];
      volatile uint8_t brightness_;
  };
}
#endif
