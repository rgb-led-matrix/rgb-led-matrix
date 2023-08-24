#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <mutex>
#include <stdint.h>
#include "CFG/CFG.h"
#include "Panel/Panel.h"
using std::mutex;

namespace rgb_matrix {
  template <typename T> class Framebuffer : public Panel {
    public:
      virtual ~Framebuffer();

      static Framebuffer *CreateFramebuffer(CFG *cfg);

      void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
      void show();
      void set_brightness(uint8_t brightness);
      cord_t get_size();
      void map_wavelength(uint8_t color, Color index, uint16_t value);

    protected:
      Framebuffer();
      Framebuffer(CFG *cfg);

      void build_table();

      virtual void show_internal() = 0;
      virtual void  MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, T *color) = 0;

      CFG *cfg_;
      T **buffer_;
      T lut[100][256];
      volatile uint8_t brightness_;
      mutex lock_;
  };
}
#endif
