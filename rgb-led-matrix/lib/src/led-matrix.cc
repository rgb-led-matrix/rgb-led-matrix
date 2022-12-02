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

#include "led-matrix.h"

#include <assert.h>
#include <grp.h>
#include <pwd.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "framecanvas.h"

#include "mappers/multiplex/multiplex-mappers-internal.h"
#include "mappers/pixel/pixel-mapper.h"

#include "framebuffer/framebuffer.h"
#include "framebuffer/RP2040/RP2040.h"

namespace rgb_matrix {
RGBMatrix *RGBMatrix::_ptr = nullptr;

// TODO: Get rid of this!!!
using namespace internal;

// Some defaults. See options-initialize.cc for the command line parsing.
RGBMatrix::Options::Options() :
  hardware_mapping("regular"),
  rows(32),
  cols(32),
  pwm_bits(Framebuffer::kDefaultBitPlanes),
  brightness(100),
  multiplexing(0),
  pixel_mapper_config(NULL)
{
  // Nothing to see here.
}

RGBMatrix::RGBMatrix(Options o) :_options(o) {
  const MultiplexMapper *multiplex_mapper = NULL;
  if (_options.multiplexing > 0) {
    const MuxMapperList &multiplexers = GetRegisteredMultiplexMappers();
    if (_options.multiplexing <= (int) multiplexers.size()) {
      // TODO: we could also do a find-by-name here, but not sure if worthwhile
      multiplex_mapper = multiplexers[_options.multiplexing - 1];
    }
  }

  if (multiplex_mapper) {
    // The multiplexers might choose to have a different physical layout.
    // We need to configure that first before setting up the hardware.
    multiplex_mapper->EditColsRows(&_options.cols, &_options.rows);
  }

  Framebuffer::InitHardwareMapping(_options.hardware_mapping);
  Framebuffer::InitSharedMapper(multiplex_mapper, _options.pixel_mapper_config);
}

RGBMatrix *RGBMatrix::CreateFromOptions(Options &options) {
  if (_ptr == nullptr) {
    _ptr = new RGBMatrix(options);
  }

  return _ptr;
}

static FrameCanvas *_canvas;

Canvas *RGBMatrix::CreateCanvas(Canvas_ID id) {
  switch (id) {
    case Canvas_ID::RP2040:
      _canvas = new FrameCanvas(new RP2040(_options.rows, _options.cols, &shared_pixel_mapper_));
      break;
  }

  _canvas->framebuffer()->SetPWMBits(_options.pwm_bits);
  _canvas->framebuffer()->SetBrightness(_options.brightness);

  return _canvas;
}

void RGBMatrix::show(Canvas *c) {
  
}
};
