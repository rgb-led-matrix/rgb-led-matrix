// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Copyright (C) 2013 Henner Zeller <h.zeller@acm.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://gnu.org/licenses/gpl-2.0.txt>
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <stdlib.h>
#include "canvas.h"
#include "port/pin-mapper/PinMapping.h"
#include "mappers/multiplex/multiplex-mappers-internal.h"

namespace rgb_matrix {
struct PixelDesignator {
  PixelDesignator() : r_bit(0), g_bit(0), b_bit(0) {}

  uint16_t r_bit;
  uint16_t g_bit;
  uint16_t b_bit;
};

class PixelDesignatorMap {
public:
  PixelDesignatorMap(int width, int height);
  ~PixelDesignatorMap();

  PixelDesignator *get(int x, int y);

  inline int width() const { return width_; }
  inline int height() const { return height_; }
  inline PixelDesignator *const buffer() const { return buffer_; }

private:
  const int width_;
  const int height_;
  PixelDesignator *const buffer_;
};

class Framebuffer {
public:
  static constexpr int kBitPlanes = 11;
  static constexpr int kDefaultBitPlanes = 11;

  Framebuffer(int rows, int columns);
  virtual ~Framebuffer() {}

  static void InitHardwareMapping(const char *named_hardware);
  static Framebuffer *CreateFramebuffer(Canvas_ID id, int rows, int cols);

  virtual bool SetPWMBits(uint8_t value);

  // Set brightness in percent; range=1..100
  void SetBrightness(uint8_t b) {
    brightness_ = (b <= 100 ? (b != 0 ? b : 1) : 100);
  }

  virtual int width() const;
  virtual int height() const;
  virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
  virtual void Serialize(const char **data, size_t *len, Canvas_ID *id);
  virtual bool Deserialize(const char *data, size_t len, Canvas_ID id);

  virtual void DumpToMatrix() = 0;

  bool ApplyPixelMapper(const PixelMapper *mapper);
  void ApplyNamedPixelMappers(const char *pixel_mapper_config);
  void InitSharedMapper(const internal::MultiplexMapper *multiplex_mapper, const char *pixel_mapper_config);

protected:
  Framebuffer();

  virtual void InitGPIO() = 0;
  virtual inline void  MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) = 0;

  const int rows_;  
  const int columns_;
  uint8_t pwm_bits_; 
  uint8_t brightness_;
  Canvas_ID id_;
  PixelDesignatorMap **shared_mapper_;

  static const struct PinMapping *hardware_mapping_;
};
}
#endif
