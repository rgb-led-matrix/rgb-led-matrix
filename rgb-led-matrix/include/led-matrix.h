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

namespace rgb_matrix {
class RGBMatrix;
class FrameCanvas;   // Canvas for Double- and Multibuffering
struct RuntimeOptions;

// The RGB matrix provides the framebuffer and the facilities to constantly
// update the LED matrix.
//
// This implement the Canvas interface that represents the display with
// (led_cols * chained_displays)x(rows * parallel_displays) pixels.
//
// If can do multi-buffering using the CreateFrameCanvas() and SwapOnVSync()
// methods. This is useful for animations and to prevent tearing.
//
// If you arrange the panels in a different way in the physical space, write
// a CanvasTransformer that does coordinate remapping and which should be added
// to the transformers, like with UArrangementTransformer in demo-main.cc.
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

    // The chain_length is the number of displays daisy-chained together
    // (output of one connected to input of next). Default: 1
    // Flag: --led-chain
    int chain_length;

    // The number of parallel chains connected to the Pi; in old Pis with 26
    // GPIO pins, that is 1, in newer Pis with 40 interfaces pins, that can
    // also be 2 or 3. The effective number of pixels in vertical direction is
    // then thus rows * parallel. Default: 1
    // Flag: --led-parallel
    int parallel;

    // Set PWM bits used for output. Default is 11, but if you only deal with
    // limited comic-colors, 1 might be sufficient. Lower require less CPU and
    // increases refresh-rate.
    // Flag: --led-pwm-bits
    int pwm_bits;

    // Change the base time-unit for the on-time in the lowest
    // significant bit in nanoseconds.
    // Higher numbers provide better quality (more accurate color, less
    // ghosting), but have a negative impact on the frame rate.
    // Flag: --led-pwm-lsb-nanoseconds
    int pwm_lsb_nanoseconds;

    // The lower bits can be time-dithered for higher refresh rate.
    // Flag: --led-pwm-dither-bits
    int pwm_dither_bits;

    // The initial brightness of the panel in percent. Valid range is 1..100
    // Default: 100
    // Flag: --led-brightness
    int brightness;

    // Default row address type is 0, corresponding to direct setting of the
    // row, while row address type 1 is used for panels that only have A/B,
    // typically some 64x64 panels
    int row_address_type;  // Flag --led-row-addr-type

    // Type of multiplexing. 0 = direct, 1 = stripe, 2 = checker,...
    // Flag: --led-multiplexing
    int multiplexing;

    // Show refresh rate on the terminal for debugging and tweaking purposes.
    bool show_refresh_rate;            // Flag: --led-show-refresh

    // A string describing a sequence of pixel mappers that should be applied
    // to this matrix. A semicolon-separated list of pixel-mappers with optional
    // parameter.
    const char *pixel_mapper_config;   // Flag: --led-pixel-mapper

    // Limit refresh rate of LED panel. This will help on a loaded system
    // to keep a constant refresh rate. <= 0 for no limit.
    int limit_refresh_rate_hz;   // Flag: --led-limit-refresh
  };

  // Factory to create a matrix. Additional functionality includes dropping
  // privileges and becoming a daemon.
  // Returns NULL, if there was a problem (a message then is written to stderr).
  static RGBMatrix *CreateFromOptions(const Options &options,
                                      const RuntimeOptions &runtime_options);

  // Stop matrix, delete all resources.
  virtual ~RGBMatrix();

  // -- Double- and Multibuffering.

  // Create a new buffer to be used for multi-buffering. The returned new
  // Buffer implements a Canvas with the same size of thie RGBMatrix.
  // You can use it to draw off-screen on it, then swap it with the active
  // buffer using SwapOnVSync(). That would be classic double-buffering.
  //
  // You can also create as many FrameCanvas as you like and for instance use
  // them to pre-fill scenes of an animation for fast playback later.
  //
  // The ownership of the created Canvases remains with the RGBMatrix, so you
  // don't have to worry about deleting them (but you also don't want to create
  // more than needed as this will fill up your memory as they are only deleted
  // when the RGBMatrix is deleted).
  FrameCanvas *CreateFrameCanvas();

  // This method waits to the next VSync and swaps the active buffer with the
  // supplied buffer. The formerly active buffer is returned.
  //
  // If you pass in NULL, the active buffer is returned, but it won't be
  // replaced with NULL. You can use the NULL-behavior to just wait on
  // VSync or to retrieve the initial buffer when preparing a multi-buffer
  // animation.
  //
  // The optional "framerate_fraction" parameter allows to choose which
  // multiple of the global frame-count to use. So it slows down your animation
  // to an exact integer fraction of the refresh rate.
  // Default is 1, so immediately next available frame.
  // (Say you have 140Hz refresh rate, then a value of 5 would give you an
  // 28Hz animation, nicely locked to the refresh-rate).
  // If you combine this with Options::limit_refresh_rate_hz you can create
  // time-correct animations.
  FrameCanvas *SwapOnVSync(FrameCanvas *other, unsigned framerate_fraction = 1);

  // -- Setting shape and behavior of matrix.

  // Apply a pixel mapper. This is used to re-map pixels according to some
  // scheme implemented by the PixelMapper. Does _not_ take ownership of the
  // mapper. Mapper can be NULL, in which case nothing happens.
  // Returns a boolean indicating if this was successful.
  bool ApplyPixelMapper(const PixelMapper *mapper);

  //--  Rarely needed
  // Start the refresh thread.
  // This is only needed if you chose RuntimeOptions::daemon = -1 (see below),
  // otherwise the refresh thread is already started.
  bool StartRefresh();

private:
  class Impl;

  RGBMatrix(Impl *impl) : impl_(impl) {}
  Impl *const impl_;
};



// Runtime options to simplify doing common things for many programs such as
// dropping privileges and becoming a daemon.
struct RuntimeOptions {
  RuntimeOptions();

  int gpio_slowdown;    // 0 = no slowdown.          Flag: --led-slowdown-gpio

  // By default, the gpio is initialized for you, but if you run on a platform
  // not the Raspberry Pi, this will fail. If you don't need to access GPIO
  // e.g. you want to just create a stream output (see content-streamer.h),
  // set this to false.
  bool do_gpio_init;
};
}  // end namespace rgb_matrix
#endif  // RPI_RGBMATRIX_H
