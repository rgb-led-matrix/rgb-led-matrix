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

#ifndef RPI_RGBMATRIX_H
#define RPI_RGBMATRIX_H

#include <stdint.h>
#include <stddef.h>
#include "canvas.h"

namespace rgb_matrix {
class RGBMatrix {
public:
  // Options to initialize the RGBMatrix. Also see the main README.md for
  // detailed descriptions of the command line flags.
  struct Options {
    Options();   // Creates a default option set.

    // Name of the hardware mapping. Something like "regular" or "adafruit-hat"
    const char *hardware_mapping;

    // The "rows" are the number
    // of rows supported by the display, so 32 or 16. Default: 32.
    // Flag: --led-rows
    int rows;

    // The "cols" are the number of columns per panel. Typically something
    // like 32, but also 64 is possible. Sometimes even 40.
    // cols * chain_length is the total length of the display, so you can
    // represent a 64 wide display as cols=32, chain=2 or cols=64, chain=1;
    // same thing, but more convenient to think of.
    // Flag: --led-cols
    int cols;

    // Set PWM bits used for output. Default is 11, but if you only deal with
    // limited comic-colors, 1 might be sufficient. Lower require less CPU and
    // increases refresh-rate.
    // Flag: --led-pwm-bits
    int pwm_bits;

    // The initial brightness of the panel in percent. Valid range is 1..100
    // Default: 100
    // Flag: --led-brightness
    int brightness;

    // Type of multiplexing. 0 = direct, 1 = stripe, 2 = checker,...
    // Flag: --led-multiplexing
    int multiplexing;

    // A string describing a sequence of pixel mappers that should be applied
    // to this matrix. A semicolon-separated list of pixel-mappers with optional
    // parameter.
    const char *pixel_mapper_config;   // Flag: --led-pixel-mapper
  };

  static RGBMatrix *CreateFromOptions(Options &options);
  virtual ~RGBMatrix();

  virtual Canvas *CreateCanvas(Canvas_ID id);

protected:

  Options _options;
  static RGBMatrix *_ptr;

private:
  RGBMatrix() {}
  RGBMatrix(Options o);
};
}  // end namespace rgb_matrix
#endif  // RPI_RGBMATRIX_H
