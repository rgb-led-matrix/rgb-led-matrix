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

const struct PinMapping *Framebuffer::hardware_mapping_ = NULL;

// Don't use this!    
Framebuffer::Framebuffer()
  : rows_(16),
    columns_(32),
    pwm_bits_(kBitPlanes), do_luminance_correct_(true), brightness_(100),
    shared_mapper_(NULL) {
  assert(shared_mapper_ != NULL);
}

Framebuffer::Framebuffer(int rows, int columns, PixelDesignatorMap **mapper)
  : rows_(rows),
    columns_(columns),
    pwm_bits_(kBitPlanes), do_luminance_correct_(true), brightness_(100),
    shared_mapper_(mapper) {
  assert(hardware_mapping_ != NULL);   // Called InitHardwareMapping() ?
  assert(shared_mapper_ != NULL);  // Storage should be provided by RGBMatrix.
}

Framebuffer::~Framebuffer() {
  delete [] bitplane_buffer_;
}

/* static */ void Framebuffer::InitHardwareMapping(const char *named_hardware) {
  if (named_hardware == NULL || *named_hardware == '\0') {
    named_hardware = "regular";
  }

  struct PinMapping *mapping = NULL;
  for (PinMapping *it = pin_mappings; it->name; ++it) {
    if (strcasecmp(it->name, named_hardware) == 0) {
      mapping = it;
      break;
    }
  }

  if (!mapping) {
    fprintf(stderr, "There is no hardware mapping named '%s'.\nAvailable: ",
            named_hardware);
    for (PinMapping *it = pin_mappings; it->name; ++it) {
      if (it != pin_mappings) fprintf(stderr, ", ");
      fprintf(stderr, "'%s'", it->name);
    }
    fprintf(stderr, "\n");
    abort();
  }
  hardware_mapping_ = mapping;
}

/*void Framebuffer::InitGPIO(GPIO *io) {
  const struct PinMapping &h = *hardware_mapping_;
  gpio_bits_t all_used_bits = 0;
  all_used_bits |= h.TX | h.RX | h.reset | h.bootloader | h.cs_out;
  for (int i = 0; i < sizeof(hardware_mapping_->address) / sizeof(gpio_bits_t); i++)
    all_used_bits |= h.address[i];

  // Initialize outputs, make sure that all of these are supported bits.
  const gpio_bits_t result = io->InitOutputs(all_used_bits);
  assert(result == all_used_bits);  // Impl: all bits declared in gpio.cc ?
}*/

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
