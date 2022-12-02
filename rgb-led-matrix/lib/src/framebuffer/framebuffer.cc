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

#include "framebuffer/framebuffer.h"
#include "port/gpio/gpio.h"

namespace rgb_matrix {
// We need one global instance of a timing correct pulser. There are different
// implementations depending on the context.
static PinPulser *sOutputEnablePulser = NULL;

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

const struct HardwareMapping *Framebuffer::hardware_mapping_ = NULL;

// Don't use it this!    
Framebuffer::Framebuffer()
  : rows_(16),
    parallel_(1),
    height_(16 * 1),
    columns_(32),
    pwm_bits_(kBitPlanes), do_luminance_correct_(true), brightness_(100),
    double_rows_(16 / SUB_PANELS_),
    shared_mapper_(NULL) {
  assert(shared_mapper_ != NULL);
}

Framebuffer::Framebuffer(int rows, int columns, int parallel, PixelDesignatorMap **mapper)
  : rows_(rows),
    parallel_(parallel),
    height_(rows * parallel),
    columns_(columns),
    pwm_bits_(kBitPlanes), do_luminance_correct_(true), brightness_(100),
    double_rows_(rows / SUB_PANELS_),
    shared_mapper_(mapper) {
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
}

Framebuffer::~Framebuffer() {
  delete [] bitplane_buffer_;
}

// TODO: this should also be parsed from some special formatted string, e.g.
// {addr={22,23,24,25,15},oe=18,clk=17,strobe=4, p0={11,27,7,8,9,10},...}
/* static */ void Framebuffer::InitHardwareMapping(const char *named_hardware) {
  if (named_hardware == NULL || *named_hardware == '\0') {
    named_hardware = "regular";
  }

  struct HardwareMapping *mapping = NULL;
  for (HardwareMapping *it = matrix_hardware_mappings; it->name; ++it) {
    if (strcasecmp(it->name, named_hardware) == 0) {
      mapping = it;
      break;
    }
  }

  if (!mapping) {
    fprintf(stderr, "There is no hardware mapping named '%s'.\nAvailable: ",
            named_hardware);
    for (HardwareMapping *it = matrix_hardware_mappings; it->name; ++it) {
      if (it != matrix_hardware_mappings) fprintf(stderr, ", ");
      fprintf(stderr, "'%s'", it->name);
    }
    fprintf(stderr, "\n");
    abort();
  }

  if (mapping->max_parallel_chains == 0) {
    // Auto determine.
    struct HardwareMapping *h = mapping;
    if ((h->p0_r1 | h->p0_g1 | h->p0_g1 | h->p0_r2 | h->p0_g2 | h->p0_g2) > 0)
      ++mapping->max_parallel_chains;
    if ((h->p1_r1 | h->p1_g1 | h->p1_g1 | h->p1_r2 | h->p1_g2 | h->p1_g2) > 0)
      ++mapping->max_parallel_chains;
    if ((h->p2_r1 | h->p2_g1 | h->p2_g1 | h->p2_r2 | h->p2_g2 | h->p2_g2) > 0)
      ++mapping->max_parallel_chains;
    if ((h->p3_r1 | h->p3_g1 | h->p3_g1 | h->p3_r2 | h->p3_g2 | h->p3_g2) > 0)
      ++mapping->max_parallel_chains;
    if ((h->p4_r1 | h->p4_g1 | h->p4_g1 | h->p4_r2 | h->p4_g2 | h->p4_g2) > 0)
      ++mapping->max_parallel_chains;
    if ((h->p5_r1 | h->p5_g1 | h->p5_g1 | h->p5_r2 | h->p5_g2 | h->p5_g2) > 0)
      ++mapping->max_parallel_chains;
  }
  hardware_mapping_ = mapping;
}

/* static */ void Framebuffer::InitGPIO(GPIO *io, int rows, int parallel, int row_address_type, int refresh) {
  if (sOutputEnablePulser != NULL)
    return;  // already initialized.

  const struct HardwareMapping &h = *hardware_mapping_;
  gpio_bits_t all_used_bits = 0;
  all_used_bits |= h.trigger | h.response | h.clock | h.strobe;
  all_used_bits |= h.p0_r1 | h.p0_g1 | h.p0_b1 | h.p0_r2 | h.p0_g2 | h.p0_b2;
  if (parallel >= 2)
    all_used_bits |= h.p1_r1 | h.p1_g1 | h.p1_b1 | h.p1_r2 | h.p1_g2 | h.p1_b2;
  if (parallel >= 3)
    all_used_bits |= h.p2_r1 | h.p2_g1 | h.p2_b1 | h.p2_r2 | h.p2_g2 | h.p2_b2;
  if (parallel >= 4)
    all_used_bits |= h.p3_r1 | h.p3_g1 | h.p3_b1 | h.p3_r2 | h.p3_g2 | h.p3_b2;
  if (parallel >= 5) 
    all_used_bits |= h.p4_r1 | h.p4_g1 | h.p4_b1 | h.p4_r2 | h.p4_g2 | h.p4_b2;
  if (parallel >= 6)
    all_used_bits |= h.p5_r1 | h.p5_g1 | h.p5_b1 | h.p5_r2 | h.p5_g2 | h.p5_b2;

  const int double_rows = rows / SUB_PANELS_;

  // Initialize outputs, make sure that all of these are supported bits.
  const gpio_bits_t result = io->InitOutputs(all_used_bits);
  assert(result == all_used_bits);  // Impl: all bits declared in gpio.cc ?

  sOutputEnablePulser = PinPulser::Create(io, h.trigger,  h.response, refresh, row_address_type);
}

bool Framebuffer::SetPWMBits(uint8_t value) {
  if (value < 1 || value > kBitPlanes)
    return false;
  pwm_bits_ = value;
  return true;
}

int Framebuffer::width() const { return (*shared_mapper_)->width(); }
int Framebuffer::height() const { return (*shared_mapper_)->height(); }

// TODO: Fix this
void Framebuffer::Serialize(const char **data, size_t *len, Canvas_ID *id) const {
  *data = reinterpret_cast<const char*>(bitplane_buffer_);
  *len = buffer_size_;
  *id = id_;
}

// TODO: Fix this
bool Framebuffer::Deserialize(const char *data, size_t len, Canvas_ID id) {
  if (len != buffer_size_ || id != id_) return false;
  memcpy(bitplane_buffer_, data, len);
  return true;
}
}  // namespace rgb_matrix
