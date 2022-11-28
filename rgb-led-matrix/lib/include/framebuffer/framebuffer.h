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
#include "port/pin-mapper/hardware-mapping.h"
#include "canvas.h"

namespace rgb_matrix {
class GPIO;
class PinPulser;

// An opaque type used within the framebuffer that can be used
// to copy between PixelMappers.
struct PixelDesignator {
  PixelDesignator() : gpio_word(-1), r_bit(0), g_bit(0), b_bit(0), mask(~0u){}
  long gpio_word;
  gpio_bits_t r_bit;
  gpio_bits_t g_bit;
  gpio_bits_t b_bit;
  gpio_bits_t mask;
};

class PixelDesignatorMap {
public:
  PixelDesignatorMap(int width, int height);
  ~PixelDesignatorMap();

  // Get a writable version of the PixelDesignator. Outside Framebuffer used
  // by the RGBMatrix to re-assign mappings to new PixelDesignatorMappers.
  PixelDesignator *get(int x, int y);

  inline int width() const { return width_; }
  inline int height() const { return height_; }

private:
  const int width_;
  const int height_;
  PixelDesignator *const buffer_;
};

// Internal representation of the frame-buffer that as well can
// write itself to GPIO.
// Our internal memory layout mimicks as much as possible what needs to be
// written out.
class Framebuffer {
public:
  // Maximum usable bitplanes.
  //
  // 11 bits seems to be a sweet spot in which we still get somewhat useful
  // refresh rate and have good color richness. This is the default setting
  // However, in low-light situations, we want to be able to scale down
  // brightness more, having more bits at the bottom.
  // TODO(hzeller): make the default 15 bit or so, but slide the use of
  //  timing to lower bits if fewer bits requested to not affect the overall
  //  refresh in that case.
  //  This needs to be balanced to not create too agressive timing however.
  //  To be explored in a separete commit.
  //
  // For now, if someone needs very low level of light, change this to
  // say 13 and recompile. Run with --led-pwm-bits=13. Also, consider
  // --led-pwm-dither-bits=2 to have the refresh rate not suffer too much.
  static constexpr int kBitPlanes = 11;
  static constexpr int kDefaultBitPlanes = 11;

  Framebuffer(int rows, int columns, int parallel, PixelDesignatorMap **mapper);
  ~Framebuffer();

  // Initialize GPIO bits for output. Only call once.
  static void InitHardwareMapping(const char *named_hardware);
  static void InitGPIO(GPIO *io, int rows, int parallel, int row_address_type, int refresh);

  // Set PWM bits used for output. Default is 11, but if you only deal with
  // simple comic-colors, 1 might be sufficient. Lower require less CPU.
  // Returns boolean to signify if value was within range.
  virtual bool SetPWMBits(uint8_t value);
  virtual uint8_t pwmbits() { return pwm_bits_; }

  // Map brightness of output linearly to input with CIE1931 profile.
  void set_luminance_correct(bool on) { do_luminance_correct_ = on; }
  bool luminance_correct() const { return do_luminance_correct_; }

  // Set brightness in percent; range=1..100
  // This will only affect newly set pixels.
  void SetBrightness(uint8_t b) {
    brightness_ = (b <= 100 ? (b != 0 ? b : 1) : 100);
  }
  uint8_t brightness() { return brightness_; }

  virtual void DumpToMatrix(GPIO *io, int pwm_bits_to_show) = 0;

  virtual void Serialize(const char **data, size_t *len, Canvas_ID *id) const;
  virtual bool Deserialize(const char *data, size_t len, Canvas_ID id);

  // Canvas-inspired methods, but we're not implementing this interface to not
  // have an unnecessary vtable.
  virtual int width() const;
  virtual int height() const;
  virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) = 0;

protected:
  Framebuffer();

  const int rows_;     // Number of rows. 16 or 32.
  const int parallel_; // Parallel rows of chains. 1 or 2.
  const int height_;   // rows * parallel
  const int columns_;  // Number of columns. Number of chained boards * 32.

  uint8_t pwm_bits_;   // PWM bits to display.
  bool do_luminance_correct_;
  uint8_t brightness_;
  Canvas_ID id_;

  const int double_rows_;
  size_t buffer_size_;

  static const struct HardwareMapping *hardware_mapping_;

  PixelDesignatorMap **shared_mapper_;  // Storage in RGBMatrix.

  // The frame-buffer is organized in bitplanes.
  // Highest level (slowest to cycle through) are double rows.
  // For each double-row, we store pwm-bits columns of a bitplane.
  // Each bitplane-column is pre-filled IoBits, of which the colors are set.
  // Of course, that means that we store unrelated bits in the frame-buffer,
  // but it allows easy access in the critical section.
  gpio_bits_t *bitplane_buffer_;
  virtual inline gpio_bits_t *ValueAt(int double_row, int column, int bit) = 0;
  virtual inline void  MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) = 0;
};

#ifdef ONLY_SINGLE_SUB_PANEL
#  define SUB_PANELS_ 1
#else
#  define SUB_PANELS_ 2
#endif

}  // namespace rgb_matrix
#endif // RPI_RGBMATRIX_FRAMEBUFFER_INTERNAL_H
