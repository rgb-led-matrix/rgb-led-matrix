#include <stdlib.h>
#include "framebuffer/PixelDesignatorMap.h"
#include "framebuffer/external/external.h"

namespace rgb_matrix {
  template <typename T> T *PixelDesignatorMap<T>::get(uint32_t location) {
    if (location >= height_ * width_)
      return NULL;
    return buffer_ + location;
  }

  template <typename T> uint32_t *PixelDesignatorMap<T>::get(int x, int y) {
    if (x < 0 || y < 0 || x >= width_ || y >= height_)
      return 0;
    return locations_ + (y * width_) + x;
  }

  template <typename T> PixelDesignatorMap<T>::PixelDesignatorMap(int width, int height)
    : width_(width), height_(height), buffer_(new T[width * height]), locations_(new uint32_t[width * height]) {
        for (int y = 0; y < height; ++y)
          for (int x = 0; x < width; ++x)
            locations_[y * width + x] = y * width + x;
  }

  template <typename T> PixelDesignatorMap<T>::~PixelDesignatorMap() {
    delete [] buffer_;
  }

  template class PixelDesignatorMap<PixelDesignator>;

}  // namespace rgb_matrix
