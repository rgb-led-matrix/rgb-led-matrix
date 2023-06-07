#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include "mappers/MultiplexMapper.h"
#include "PixelDesignatorMap.h"
#include "CFG/CFG.h"

namespace rgb_matrix {
  template <typename T> class Framebuffer {
    public:
      Framebuffer(CFG *cfg);
      virtual ~Framebuffer() {}

      static Framebuffer *CreateFramebuffer(Options options, const MultiplexMapper *multiplex_mapper);

      virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);

      virtual void DumpToMatrix() = 0;

      bool ApplyPixelMapper(const MultiplexMapper *mapper);
      void InitSharedMapper(const MultiplexMapper *multiplex_mapper);

    protected:
      Framebuffer();

      virtual void  MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) = 0;

      CFG *cfg_;
      PixelDesignatorMap<T> *shared_mapper_;
  };
}
#endif
