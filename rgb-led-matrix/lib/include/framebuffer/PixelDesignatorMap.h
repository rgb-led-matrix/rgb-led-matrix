#ifndef PIXELDESIGNATORMAP_H
#define PIXELDESIGNATORMAP_H

#include <stdint.h>

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
}
#endif
