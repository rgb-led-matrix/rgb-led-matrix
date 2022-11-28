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

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "framebuffer/BCM/BCM.h"
#include "port/gpio/gpio.h"
#include "pin-pulser/pin-pulser.h"

namespace rgb_matrix {
// We need one global instance of a timing correct pulser. There are different
// implementations depending on the context.
static PinPulser *sOutputEnablePulser = NULL;

#ifdef ONLY_SINGLE_SUB_PANEL
#  define SUB_PANELS_ 1
#else
#  define SUB_PANELS_ 2
#endif

PixelDesignator *PixelDesignatorMap::get(int x, int y) {
  if (x < 0 || y < 0 || x >= width_ || y >= height_)
    return NULL;
  return buffer_ + (y*width_) + x;
}

PixelDesignatorMap::PixelDesignatorMap(int width, int height)
  : width_(width), height_(height),
    buffer_(new PixelDesignator[width * height]) {
}

PixelDesignatorMap::~PixelDesignatorMap() {
  delete [] buffer_;
}

BCM::BCM(int rows, int columns, int parallel, PixelDesignatorMap **mapper)
  : Framebuffer(rows, columns, parallel, mapper) {
  assert(hardware_mapping_ != NULL);   // Called InitHardwareMapping() ?
  assert(shared_mapper_ != NULL);  // Storage should be provided by RGBMatrix.
  assert(rows_ >=4 && rows_ <= 64 && rows_ % 2 == 0);
  if (parallel > hardware_mapping_->max_parallel_chains) {
    fprintf(stderr, "The %s GPIO mapping only supports %d parallel chain%s, "
            "but %d was requested.\n", hardware_mapping_->name,
            hardware_mapping_->max_parallel_chains,
            hardware_mapping_->max_parallel_chains > 1 ? "s" : "", parallel);
    abort();
  }
  assert(parallel >= 1 && parallel <= 6);

  bitplane_buffer_ = new gpio_bits_t[double_rows_ * columns_ * kBitPlanes];

  // If we're the first Framebuffer created, the shared PixelMapper is
  // still NULL, so create one.
  // The first PixelMapper represents the physical layout of a standard matrix
  // with the specific knowledge of the framebuffer, setting up PixelDesignators
  // in a way that they are useful for this Framebuffer.
  //
  // Newly created PixelMappers then can just re-arrange PixelDesignators
  // from the parent PixelMapper opaquely without having to know the details.
  if (*shared_mapper_ == NULL) {
    // Gather all the bits for given color for fast Fill()s and use the right
    // bits according to the led sequence
    const struct HardwareMapping &h = *hardware_mapping_;
    gpio_bits_t r = h.p0_r1 | h.p0_r2 | h.p1_r1 | h.p1_r2 | h.p2_r1 | h.p2_r2 | h.p3_r1 | h.p3_r2 | h.p4_r1 | h.p4_r2 | h.p5_r1 | h.p5_r2;
    gpio_bits_t g = h.p0_g1 | h.p0_g2 | h.p1_g1 | h.p1_g2 | h.p2_g1 | h.p2_g2 | h.p3_g1 | h.p3_g2 | h.p4_g1 | h.p4_g2 | h.p5_g1 | h.p5_g2;
    gpio_bits_t b = h.p0_b1 | h.p0_b2 | h.p1_b1 | h.p1_b2 | h.p2_b1 | h.p2_b2 | h.p3_b1 | h.p3_b2 | h.p4_b1 | h.p4_b2 | h.p5_b1 | h.p5_b2;

    *shared_mapper_ = new PixelDesignatorMap(columns_, height_);
    for (int y = 0; y < height_; ++y) {
      for (int x = 0; x < columns_; ++x) {
        InitDefaultDesignator(x, y, (*shared_mapper_)->get(x, y));
      }
    }
  }
}

BCM::~BCM() {
  delete [] bitplane_buffer_;
}

inline gpio_bits_t *BCM::ValueAt(int double_row, int column, int bit) {
  return &bitplane_buffer_[ double_row * (columns_ * kBitPlanes)
                            + bit * columns_
                            + column ];
}

// Do CIE1931 luminance correction and scale to output bitplanes
static uint16_t luminance_cie1931(uint8_t c, uint8_t brightness) {
  float out_factor = ((1 << Framebuffer::kBitPlanes) - 1);
  float v = (float) c * brightness / 255.0;
  return roundf(out_factor * ((v <= 8) ? v / 902.3 : pow((v + 16) / 116.0, 3)));
}

struct ColorLookup {
  uint16_t color[256];
};
static ColorLookup *CreateLuminanceCIE1931LookupTable() {
  ColorLookup *for_brightness = new ColorLookup[100];
  for (int c = 0; c < 256; ++c)
    for (int b = 0; b < 100; ++b)
      for_brightness[b].color[c] = luminance_cie1931(c, b + 1);

  return for_brightness;
}

static inline uint16_t CIEMapColor(uint8_t brightness, uint8_t c) {
  static ColorLookup *luminance_lookup = CreateLuminanceCIE1931LookupTable();
  return luminance_lookup[brightness - 1].color[c];
}

// Non luminance correction. TODO: consider getting rid of this.
static inline uint16_t DirectMapColor(uint8_t brightness, uint8_t c) {
  // simple scale down the color value
  c = c * brightness / 100;

  // shift to be left aligned with top-most bits.
  constexpr int shift = Framebuffer::kBitPlanes - 8;
  return (shift > 0) ? (c << shift) : (c >> -shift);
}

inline void BCM::MapColors(
  uint8_t r, uint8_t g, uint8_t b,
  uint16_t *red, uint16_t *green, uint16_t *blue) {

  if (do_luminance_correct_) {
    *red   = CIEMapColor(brightness_, r);
    *green = CIEMapColor(brightness_, g);
    *blue  = CIEMapColor(brightness_, b);
  } else {
    *red   = DirectMapColor(brightness_, r);
    *green = DirectMapColor(brightness_, g);
    *blue  = DirectMapColor(brightness_, b);
  }
}

void BCM::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
  const PixelDesignator *designator = (*shared_mapper_)->get(x, y);
  if (designator == NULL) return;
  const long pos = designator->gpio_word;
  if (pos < 0) return;  // non-used pixel marker.

  uint16_t red, green, blue;
  MapColors(r, g, b, &red, &green, &blue);

  gpio_bits_t *bits = bitplane_buffer_ + pos;
  const int min_bit_plane = kBitPlanes - pwm_bits_;
  bits += (columns_ * min_bit_plane);
  const gpio_bits_t r_bits = designator->r_bit;
  const gpio_bits_t g_bits = designator->g_bit;
  const gpio_bits_t b_bits = designator->b_bit;
  const gpio_bits_t designator_mask = designator->mask;
  for (uint16_t mask = 1<<min_bit_plane; mask != 1<<kBitPlanes; mask <<=1 ) {
    gpio_bits_t color_bits = 0;
    if (red & mask)   color_bits |= r_bits;
    if (green & mask) color_bits |= g_bits;
    if (blue & mask)  color_bits |= b_bits;
    *bits = (*bits & designator_mask) | color_bits;
    bits += columns_;
  }
}

void BCM::InitDefaultDesignator(int x, int y, PixelDesignator *d) {
  const struct HardwareMapping &h = *hardware_mapping_;
  gpio_bits_t *bits = ValueAt(y % double_rows_, x, 0);
  d->gpio_word = bits - bitplane_buffer_;
  d->r_bit = d->g_bit = d->b_bit = 0;

  int index = y / rows_;
  int subindex = (y % rows_) / double_rows_;
  static gpio_bits_t table[][2][3] = {
    { { h.p0_r1, h.p0_g1, h.p0_b1 }, { h.p0_r2, h.p0_g2, h.p0_b2 } },
    { { h.p1_r1, h.p1_g1, h.p1_b1 }, { h.p1_r2, h.p1_g2, h.p1_b2 } },
    { { h.p2_r1, h.p2_g1, h.p2_b1 }, { h.p2_r2, h.p2_g2, h.p2_b2 } },
    { { h.p3_r1, h.p3_g1, h.p3_b1 }, { h.p3_r2, h.p3_g2, h.p3_b2 } },
    { { h.p4_r1, h.p4_g1, h.p4_b1 }, { h.p4_r2, h.p4_g2, h.p4_b2 } },
    { { h.p5_r1, h.p5_g1, h.p5_b1 }, { h.p5_r2, h.p5_g2, h.p5_b2 } }
  };

  d->r_bit = table[index][subindex][0];
  d->g_bit = table[index][subindex][1];
  d->b_bit = table[index][subindex][2];
  d->mask = ~(d->r_bit | d->g_bit | d->b_bit);
}

void BCM::DumpToMatrix(GPIO *io, int pwm_low_bit) {
  const struct HardwareMapping &h = *hardware_mapping_;
  gpio_bits_t color_clk_mask = 0;  // Mask of bits while clocking in.
  color_clk_mask |= h.p0_r1 | h.p0_g1 | h.p0_b1 | h.p0_r2 | h.p0_g2 | h.p0_b2;
  if (parallel_ >= 2)
    color_clk_mask |= h.p1_r1 | h.p1_g1 | h.p1_b1 | h.p1_r2 | h.p1_g2 | h.p1_b2;
  if (parallel_ >= 3)
    color_clk_mask |= h.p2_r1 | h.p2_g1 | h.p2_b1 | h.p2_r2 | h.p2_g2 | h.p2_b2;
  if (parallel_ >= 4)
    color_clk_mask |= h.p3_r1 | h.p3_g1 | h.p3_b1 | h.p3_r2 | h.p3_g2 | h.p3_b2;
  if (parallel_ >= 5)
    color_clk_mask |= h.p4_r1 | h.p4_g1 | h.p4_b1 | h.p4_r2 | h.p4_g2 | h.p4_b2;
  if (parallel_ >= 6)
    color_clk_mask |= h.p5_r1 | h.p5_g1 | h.p5_b1 | h.p5_r2 | h.p5_g2 | h.p5_b2;
  color_clk_mask |= h.clock;

  // Depending if we do dithering, we might not always show the lowest bits.
  const int start_bit = std::max(pwm_low_bit, kBitPlanes - pwm_bits_);
  const uint8_t half_double = double_rows_/2;
  for (uint8_t row_loop = 0; row_loop < double_rows_; ++row_loop) {
    uint8_t d_row = ((row_loop < half_double) ? (row_loop << 1) : ((row_loop - half_double) << 1) + 1);

    // TODO: Pin-Pulser will automatically hold display off during this time
    for (int b = 0; b < start_bit; ++b) {
      // OE of the previous row-data must be finished before strobe.
      sOutputEnablePulser->WaitPulseFinished();

      // Now switch on for the sleep time necessary for that bit-plane.
      sOutputEnablePulser->SendPulse();
    }

    // Rows can't be switched very quickly without ghosting, so we do the
    // full PWM of one row before switching rows.
    for (int b = start_bit; b < kBitPlanes; ++b) {
      gpio_bits_t *row_data = ValueAt(d_row, 0, b);
      // While the output enable is still on, we can already clock in the next
      // data.
      for (int col = 0; col < columns_; ++col) {
        const gpio_bits_t &out = *row_data++;
        io->WriteMaskedBits(out, color_clk_mask);  // col + reset clock
        io->SetBits(h.clock);               // Rising edge: clock color in.
      }
      io->ClearBits(color_clk_mask);    // clock back to normal.

      // OE of the previous row-data must be finished before strobe.
      sOutputEnablePulser->WaitPulseFinished();

      io->SetBits(h.strobe);   // Strobe in the previously clocked in row.
      io->ClearBits(h.strobe);

      // Now switch on for the sleep time necessary for that bit-plane.
      sOutputEnablePulser->SendPulse();
    }
  }
}
}  // namespace rgb_matrix
