#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "CFG/CFG.h"
#include "framebuffer/external/RP2040/RP2040_SPI.h"

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

  template <typename T> bool Framebuffer<T>::ApplyPixelMapper(const PixelMapper *mapper) {
    if (mapper == NULL) 
      return true;

    const int old_width = shared_mapper_->width();
    const int old_height = shared_mapper_->height();
    int new_width, new_height;

    if (!mapper->GetSizeMapping(old_width, old_height, &new_width, &new_height))
      return false;

    PixelDesignatorMap<T> *new_mapper = new PixelDesignatorMap<T>(new_width, new_height);

    for (int y = 0; y < new_height; ++y) {
      for (int x = 0; x < new_width; ++x) {
        int orig_x = -1, orig_y = -1;

        mapper->MapVisibleToMatrix(old_width, old_height, x, y, &orig_x, &orig_y);

        if (orig_x < 0 || orig_y < 0 ||
            orig_x >= old_width || orig_y >= old_height) {
          fprintf(stderr, "Error in PixelMapper: (%d, %d) -> (%d, %d) [range: "
                  "%dx%d]\n", x, y, orig_x, orig_y, old_width, old_height);
          continue;
        }

        const uint32_t *orig_designator;
        orig_designator = shared_mapper_->get(orig_x, orig_y);
        *new_mapper->get(x, y) = *orig_designator;
      }
    }

    delete shared_mapper_;
    shared_mapper_ = new_mapper;
    return true;
  }

  template <typename T> void Framebuffer<T>::InitSharedMapper(const MultiplexMapper *multiplex_mapper) {
    ApplyPixelMapper(multiplex_mapper);
  }

  template <> Framebuffer<PixelDesignator> *Framebuffer<PixelDesignator>::CreateFramebuffer(Options options, const MultiplexMapper *multiplex_mapper) {
    switch (options.get_cfg()->get_id()) {
      case Canvas_ID::RP2040_SPI_ID:
        Framebuffer<PixelDesignator> *buf = new RP2040_SPI<PixelDesignator>(options.get_cfg());
        buf->InitSharedMapper(multiplex_mapper);
        return buf;
    }

    return nullptr;
  }

  template <typename T> void Framebuffer<T>::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    T *pixel = shared_mapper_->get(*shared_mapper_->get(x, y));
    MapColors(x, y, red, green, blue, &pixel->r_bit, &pixel->g_bit, &pixel->b_bit);
  }

  template class Framebuffer<PixelDesignator>;
  template class PixelDesignatorMap<PixelDesignator>;

}  // namespace rgb_matrix
