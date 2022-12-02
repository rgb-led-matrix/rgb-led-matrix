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

// Controlling 16x32 or 32x32 RGB matrixes via GPIO. It allows daisy chaining
// of a string of these, and also connecting a parallel string on newer
// Raspberry Pis with more GPIO pins available.

#ifndef RGBMATRIX_H
#define RGBMATRIX_H

#include <stdint.h>
#include <stddef.h>
#include "canvas.h"

namespace rgb_matrix {

  struct Options {
    Options();

    const char *hardware_mapping;
    int rows;
    int cols;
    int pwm_bits;
    int brightness;
    int multiplexing;
    const char *pixel_mapper_config;
  };

  class RGBMatrix {
    public:
      virtual ~RGBMatrix() {}
      
      static RGBMatrix *CreateFromOptions(Options &options);

      virtual Canvas *CreateCanvas(Canvas_ID id);
      virtual void show(Canvas *c);

    protected:
      RGBMatrix() {}
      RGBMatrix(Options o);

      Options _options;
      static RGBMatrix *_ptr;
    };
}
#endif
