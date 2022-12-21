#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "CFG/CFG.h"
#include "framebuffer/external/RP2040/RP2040_Multiplexed_PMP.h"
#include "framebuffer/HUB75/BCM/BCM.h"
#include "port/gpio/gpio.h"
#include "mappers/pixel/pixel-mapper.h"
#include "mappers/pixel/HUB75/PixelMapper_HUB75_LUT.h"
#include "port/pin-mapper/PinMapping.h"

namespace rgb_matrix {

  template <typename T> uint32_t Framebuffer<T>::hardware_mapping_;

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

    *shared_mapper_ = new PixelDesignatorMap<T>(cfg->dot.cols, cfg->dot.rows);
  }

  template <typename T> void Framebuffer<T>::InitHardwareMapping(const char *named_hardware) {
    if (named_hardware == NULL || *named_hardware == '\0')
      named_hardware = "regular";

    for (hardware_mapping_ = 0; hardware_mapping_ < pin_mappings_size; hardware_mapping_++)
      if (strcasecmp(pin_mappings[hardware_mapping_].name, named_hardware) == 0)
        return;

    abort();
  }

  template <typename T> int Framebuffer<T>::width() const { return (*shared_mapper_)->width(); }
  template <typename T> int Framebuffer<T>::height() const { return (*shared_mapper_)->height(); }

  template <typename T> void Framebuffer<T>::ApplyNamedPixelMappers(PixelMapper_LUT *lut, const char *pixel_mapper_config) {
    if (pixel_mapper_config == NULL || strlen(pixel_mapper_config) == 0)
      return;
    char *const writeable_copy = strdup(pixel_mapper_config);
    const char *const end = writeable_copy + strlen(writeable_copy);
    char *s = writeable_copy;
    while (s < end) {
      char *const semicolon = strchrnul(s, ';');
      *semicolon = '\0';
      char *optional_param_start = strchr(s, ':');
      if (optional_param_start) {
        *optional_param_start++ = '\0';
      }
      if (*s == '\0' && optional_param_start && *optional_param_start != '\0') {
        fprintf(stderr, "Stray parameter ':%s' without mapper name ?\n", optional_param_start);
      }
      if (*s) {
        ApplyPixelMapper(lut->FindPixelMapper(s, 1, 1, optional_param_start));
      }
      s = semicolon + 1;
    }
    free(writeable_copy);
  }

  template <typename T> bool Framebuffer<T>::ApplyPixelMapper(const PixelMapper *mapper) {
    if (mapper == NULL) 
      return true;

    const int old_width = (*shared_mapper_)->width();
    const int old_height = (*shared_mapper_)->height();
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
        orig_designator = (*shared_mapper_)->get(orig_x, orig_y);
        *new_mapper->get(x, y) = *orig_designator;
      }
    }

    delete (*shared_mapper_);
    (*shared_mapper_) = new_mapper;
    return true;
  }

  template <typename T> void Framebuffer<T>::InitSharedMapper(PixelMapper_LUT *lut, const MultiplexMapper *multiplex_mapper, const char *pixel_mapper_config) {
    ApplyPixelMapper(multiplex_mapper);
    ApplyNamedPixelMappers(lut, pixel_mapper_config);
  }

  template <> Framebuffer<PixelDesignator> *Framebuffer<PixelDesignator>::CreateFramebuffer(Options options, const MultiplexMapper *multiplex_mapper) {
    switch (options.cfg->get_id()) {
      case Canvas_ID::RP2040_Multiplexed_PMP_ID:
        Framebuffer<PixelDesignator> *buf = new RP2040_Multiplexed_PMP<PixelDesignator>(options.cfg);
        // TODO: Fix nullptr
        buf->InitSharedMapper(nullptr, multiplex_mapper, options.pixel_mapper_config);
        return buf;
    }

    return nullptr;
  }

  template <> Framebuffer<PixelDesignator_HUB75> *Framebuffer<PixelDesignator_HUB75>::CreateFramebuffer(Options options, const MultiplexMapper *multiplex_mapper) {
    switch (options.cfg->get_id()) {
      case Canvas_ID::HUB75_BCM_ID:
        Framebuffer<PixelDesignator_HUB75> *buf = new BCM<PixelDesignator_HUB75>(options.cfg);
        buf->InitSharedMapper(PixelMapper_HUB75_LUT::CreateLUT(), multiplex_mapper, options.pixel_mapper_config);
        return buf;
    }

    return nullptr;
  }

  template <typename T> void Framebuffer<T>::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    T *pixel = (*shared_mapper_)->get(*(*shared_mapper_)->get(x, y));
    MapColors(red, green, blue, &pixel->r_bit, &pixel->g_bit, &pixel->b_bit);
  }

  // TODO: Fix this
  template <typename T> void Framebuffer<T>::Serialize(const char **data, size_t *len, Canvas_ID *id) {
    *data = reinterpret_cast<const char*>((*shared_mapper_)->buffer());
    *len = sizeof(T) * (*shared_mapper_)->height() * (*shared_mapper_)->width();
    *id = cfg_->get_id();
  }

  // TODO: Fix this
  template <typename T> bool Framebuffer<T>::Deserialize(const char *data, size_t len, Canvas_ID id) {
    if (len != (sizeof(T) * (*shared_mapper_)->height() * (*shared_mapper_)->width()) || id != cfg_->get_id()) 
      return false;
    memcpy((*shared_mapper_)->buffer(), data, len);
    return true;
  }

  template class Framebuffer<PixelDesignator>;
  template class PixelDesignatorMap<PixelDesignator>;
  template class Framebuffer<PixelDesignator_HUB75>;
  template class PixelDesignatorMap<PixelDesignator_HUB75>;

}  // namespace rgb_matrix
