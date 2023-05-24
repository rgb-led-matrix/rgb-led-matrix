#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <stdlib.h>
#include "Panel.h"
#include "RGBMatrix.h"
#include "external/external.h"
#include "mappers/multiplex/multiplex-mapper.h"

namespace rgb_matrix {
  template <typename T> struct PixelDesignatorMap {
    public:
      PixelDesignatorMap(int width, int height);
      ~PixelDesignatorMap();

      T *get(uint32_t location);
      uint32_t *get(int x, int y);

      inline int width() const { return width_; }
      inline int height() const { return height_; }
      inline T *const buffer() const { return buffer_; }

    private:
      const int width_;
      const int height_;
      T *const buffer_;
      uint32_t *locations_;
  };

  template <typename T> class Framebuffer {
    public:
      Framebuffer(CFG *cfg);
      virtual ~Framebuffer() {}

      static Framebuffer *CreateFramebuffer(Options options, const MultiplexMapper *multiplex_mapper);

      virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);

      virtual void DumpToMatrix() = 0;

      bool ApplyPixelMapper(const PixelMapper *mapper);
      void InitSharedMapper(const MultiplexMapper *multiplex_mapper);

    protected:
      Framebuffer();

      virtual void  MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) = 0;

      CFG *cfg_;
      PixelDesignatorMap<T> *shared_mapper_;
  };
}
#endif
