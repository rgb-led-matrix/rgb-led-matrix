#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <stdlib.h>
#include "canvas.h"
#include "led-matrix.h"
#include "external/external.h"
#include "mappers/multiplex/multiplex-mapper.h"
#include "mappers/pixel/PixelMapper_LUT.h"

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

      virtual int width() const;
      virtual int height() const;
      virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
      virtual void Serialize(const char **data, size_t *len, Canvas_ID *id);
      virtual bool Deserialize(const char *data, size_t len, Canvas_ID id);

      virtual void DumpToMatrix() = 0;

      bool ApplyPixelMapper(const PixelMapper *mapper);
      void ApplyNamedPixelMappers(PixelMapper_LUT *lut, const char *pixel_mapper_config);
      void InitSharedMapper(PixelMapper_LUT *lut, const MultiplexMapper *multiplex_mapper, const char *pixel_mapper_config);

    protected:
      Framebuffer();

      virtual void  MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) = 0;

      CFG *cfg_;

      // TODO: Remove double pointer
      PixelDesignatorMap<T> **shared_mapper_;
  };
}
#endif
