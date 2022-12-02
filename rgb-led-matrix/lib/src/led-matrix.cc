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

#include "port/gpio/gpio.h"
#include "port/thread/thread.h"
#include "framebuffer/framebuffer.h"
#include "mappers/multiplex/multiplex-mappers-internal.h"
#include "framecanvas.h"

namespace rgb_matrix {
// Implementation details of RGBmatrix.
class RGBMatrix::Impl {
  class UpdateThread;
  friend class UpdateThread;

public:
  // Create an RGBMatrix.
  //
  // Needs an initialized GPIO object and configuration options from the
  // RGBMatrix::Options struct.
  //
  // If you pass an GPIO object (which has to be Init()ialized), it will start  // the internal thread to start the screen immediately.
  //
  // If you need finer control over when the refresh thread starts (which you
  // might when you become a daemon), pass NULL here and see SetGPIO() method.
  //
  // The resulting canvas is (options.rows * options.parallel) high and
  // (32 * options.chain_length) wide.
  Impl(GPIO *io, const Options &options);

  ~Impl();

  // Used to be there to help user delay initialization of thread starting,
  // these days only used internally.
  void SetGPIO(GPIO *io, bool start_thread = true);

  bool StartRefresh();

  FrameCanvas *CreateFrameCanvas();
  FrameCanvas *SwapOnVSync(FrameCanvas *other, unsigned framerate_fraction);
  bool ApplyPixelMapper(const PixelMapper *mapper);

private:
  friend class RGBMatrix;

  // Apply pixel mappers that have been passed down via a configuration
  // string.
  void ApplyNamedPixelMappers(const char *pixel_mapper_config,
                              int chain, int parallel);

  Options params_;
  bool do_luminance_correct_;

  FrameCanvas *active_;

  GPIO *io_;
  Mutex active_frame_sync_;
  UpdateThread *updater_;
  std::vector<FrameCanvas*> created_frames_;
  PixelDesignatorMap *shared_pixel_mapper_;
  uint64_t user_output_bits_;
};

using namespace internal;

// Pump pixels to screen. Needs to be high priority real-time because jitter
class RGBMatrix::Impl::UpdateThread : public Thread {
public:
  UpdateThread(GPIO *io, FrameCanvas *initial_frame,
               int pwm_dither_bits, bool show_refresh,
               int limit_refresh_hz)
    : io_(io), show_refresh_(show_refresh),
      target_frame_usec_(limit_refresh_hz < 1 ? 0 : 1e6/limit_refresh_hz),
      running_(true),
      current_frame_(initial_frame), next_frame_(NULL),
      requested_frame_multiple_(1) {
    pthread_cond_init(&frame_done_, NULL);
    pthread_cond_init(&input_change_, NULL);
  }

  void Stop() {
    MutexLock l(&running_mutex_);
    running_ = false;
  }

  virtual void Run() {
    unsigned frame_count = 0;
    uint32_t largest_time = 0;
    gpio_bits_t last_gpio_bits = 0;

    // Let's start measure max time only after a we were running for a few
    // seconds to not pick up start-up glitches.
    static const int kHoldffTimeUs = 2000 * 1000;
    uint32_t initial_holdoff_start = GetMicrosecondCounter();
    bool max_measure_enabled = false;

    while (running()) {
      const uint32_t start_time_us = GetMicrosecondCounter();

      current_frame_->framebuffer()
        ->DumpToMatrix(io_);

      // SwapOnVSync() exchange.
      {
        MutexLock l(&frame_sync_);
        // Do fast equality test first (likely due to frame_count reset).
        if (frame_count == requested_frame_multiple_
            || frame_count % requested_frame_multiple_ == 0) {
          // We reset to avoid frame hick-up every couple of weeks
          // run-time iff requested_frame_multiple_ is not a factor of 2^32.
          frame_count = 0;
          if (next_frame_ != NULL) {
            current_frame_ = next_frame_;
            next_frame_ = NULL;
          }
          pthread_cond_signal(&frame_done_);
        }
      }

      // Read input bits.
      const gpio_bits_t inputs = io_->Read();
      if (inputs != last_gpio_bits) {
        last_gpio_bits = inputs;
        MutexLock l(&input_sync_);
        gpio_inputs_ = inputs;
        pthread_cond_signal(&input_change_);
      }

      ++frame_count;

      if (target_frame_usec_) {
        while ((GetMicrosecondCounter() - start_time_us) < target_frame_usec_) {
          // busy wait. We have our dedicated core, so ok to burn cycles.
        }
      }

      const uint32_t end_time_us = GetMicrosecondCounter();
      if (show_refresh_) {
        uint32_t usec = end_time_us - start_time_us;
        printf("\b\b\b\b\b\b\b\b%6.1fHz", 1e6 / usec);
        if (usec > largest_time && max_measure_enabled) {
          largest_time = usec;
          const float lowest_hz = 1e6 / largest_time;
          printf(" (lowest: %.1fHz)"
                 "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b", lowest_hz);
        } else {
          // Don't measure at startup, as times will be janky.
          max_measure_enabled = (end_time_us - initial_holdoff_start) > kHoldffTimeUs;
        }
      }
    }
  }

  FrameCanvas *SwapOnVSync(FrameCanvas *other, unsigned frame_fraction) {
    MutexLock l(&frame_sync_);
    FrameCanvas *previous = current_frame_;
    next_frame_ = other;
    requested_frame_multiple_ = frame_fraction;
    frame_sync_.WaitOn(&frame_done_);
    return previous;
  }

  gpio_bits_t AwaitInputChange(int timeout_ms) {
    MutexLock l(&input_sync_);
    input_sync_.WaitOn(&input_change_, timeout_ms);
    return gpio_inputs_;
  }

private:
  inline bool running() {
    MutexLock l(&running_mutex_);
    return running_;
  }

  GPIO *const io_;
  const bool show_refresh_;
  const uint32_t target_frame_usec_;

  Mutex running_mutex_;
  bool running_;

  Mutex input_sync_;
  pthread_cond_t input_change_;
  gpio_bits_t gpio_inputs_;

  Mutex frame_sync_;
  pthread_cond_t frame_done_;
  FrameCanvas *current_frame_;
  FrameCanvas *next_frame_;
  unsigned requested_frame_multiple_;
};

// Some defaults. See options-initialize.cc for the command line parsing.
RGBMatrix::Options::Options() :
  // Historically, we provided these options only as #defines. Make sure that
  // things still behave as before if someone has set these.
  // At some point: remove them from the Makefile. Later: remove them here.
#ifdef DEFAULT_HARDWARE
  hardware_mapping(DEFAULT_HARDWARE),
#else
  hardware_mapping("regular"),
#endif

  rows(32), cols(32),
  pwm_bits(Framebuffer::kDefaultBitPlanes),

  brightness(100),
  multiplexing(0),

  pixel_mapper_config(NULL),
{
  // Nothing to see here.
}

RuntimeOptions::RuntimeOptions() :
  do_gpio_init(true)
{
  // Nothing to see here.
}

RGBMatrix::Impl::Impl(GPIO *io, const Options &options)
  : params_(options), io_(NULL), updater_(NULL), shared_pixel_mapper_(NULL),
    user_output_bits_(0) {
  const MultiplexMapper *multiplex_mapper = NULL;
  if (params_.multiplexing > 0) {
    const MuxMapperList &multiplexers = GetRegisteredMultiplexMappers();
    if (params_.multiplexing <= (int) multiplexers.size()) {
      // TODO: we could also do a find-by-name here, but not sure if worthwhile
      multiplex_mapper = multiplexers[params_.multiplexing - 1];
    }
  }

  if (multiplex_mapper) {
    // The multiplexers might choose to have a different physical layout.
    // We need to configure that first before setting up the hardware.
    multiplex_mapper->EditColsRows(&params_.cols, &params_.rows);
  }

  Framebuffer::InitHardwareMapping(params_.hardware_mapping);

  active_ = CreateFrameCanvas();
  active_->Clear();
  SetGPIO(io, true);

  // We need to apply the mapping for the panels first.
  ApplyPixelMapper(multiplex_mapper);

  // .. followed by higher level mappers that might arrange panels.
  ApplyNamedPixelMappers(options.pixel_mapper_config,
                         params_.chain_length, params_.parallel);
}

RGBMatrix::Impl::~Impl() {
  if (updater_) {
    updater_->Stop();
    updater_->WaitStopped();
  }
  delete updater_;

  // Make sure LEDs are off.
  active_->Clear();
  if (io_) active_->framebuffer()->DumpToMatrix(io_);

  for (size_t i = 0; i < created_frames_.size(); ++i) {
    delete created_frames_[i];
  }
  delete shared_pixel_mapper_;
}

RGBMatrix::~RGBMatrix() {
  delete impl_;
}

void RGBMatrix::Impl::ApplyNamedPixelMappers(const char *pixel_mapper_config,
                                             int chain, int parallel) {
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
      ApplyPixelMapper(FindPixelMapper(s, chain, parallel, optional_param_start));
    }
    s = semicolon + 1;
  }
  free(writeable_copy);
}

void RGBMatrix::Impl::SetGPIO(GPIO *io, bool start_thread) {
  if (io != NULL && io_ == NULL) {
    io_ = io;
    InitGPIO(io_);
  }
  if (start_thread) {
    StartRefresh();
  }
}

bool RGBMatrix::Impl::StartRefresh() {
  if (updater_ == NULL && io_ != NULL) {
    updater_ = new UpdateThread(io_, active_, params_.pwm_dither_bits,
                                params_.show_refresh_rate,
                                params_.limit_refresh_rate_hz);
    // If we have multiple processors, the kernel
    // jumps around between these, creating some global flicker.
    // So let's tie it to the last CPU available.
    updater_->Start(99, (1<<3));  // Prio: high. Also: put on last CPU.
  }
  return updater_ != NULL;
}

FrameCanvas *RGBMatrix::Impl::CreateFrameCanvas() {
  FrameCanvas *result = NULL; // TODO: Fix this
    //new FrameCanvas(new Framebuffer(params_.rows, params_.cols * params_.chain_length, params_.parallel, &shared_pixel_mapper_));
  if (created_frames_.empty()) {
    // First time. Get defaults from initial Framebuffer.
    do_luminance_correct_ = result->framebuffer()->luminance_correct();
  }

  result->framebuffer()->SetPWMBits(params_.pwm_bits);
  result->framebuffer()->set_luminance_correct(do_luminance_correct_);
  result->framebuffer()->SetBrightness(params_.brightness);

  created_frames_.push_back(result);
  return result;
}

FrameCanvas *RGBMatrix::Impl::SwapOnVSync(FrameCanvas *other,
                                          unsigned frame_fraction) {
  if (frame_fraction == 0) frame_fraction = 1; // correct user error.
  if (!updater_) return NULL;
  FrameCanvas *const previous = updater_->SwapOnVSync(other, frame_fraction);
  if (other) active_ = other;
  return previous;
}

bool RGBMatrix::Impl::ApplyPixelMapper(const PixelMapper *mapper) {
  if (mapper == NULL) return true;
  const int old_width = shared_pixel_mapper_->width();
  const int old_height = shared_pixel_mapper_->height();
  int new_width, new_height;
  if (!mapper->GetSizeMapping(old_width, old_height, &new_width, &new_height)) {
    return false;
  }
  PixelDesignatorMap *new_mapper = new PixelDesignatorMap(new_width, new_height);
  for (int y = 0; y < new_height; ++y) {
    for (int x = 0; x < new_width; ++x) {
      int orig_x = -1, orig_y = -1;
      mapper->MapVisibleToMatrix(old_width, old_height,
                                 x, y, &orig_x, &orig_y);
      if (orig_x < 0 || orig_y < 0 ||
          orig_x >= old_width || orig_y >= old_height) {
        fprintf(stderr, "Error in PixelMapper: (%d, %d) -> (%d, %d) [range: "
                "%dx%d]\n", x, y, orig_x, orig_y, old_width, old_height);
        continue;
      }
      const PixelDesignator *orig_designator;
      orig_designator = shared_pixel_mapper_->get(orig_x, orig_y);
      *new_mapper->get(x, y) = *orig_designator;
    }
  }
  delete shared_pixel_mapper_;
  shared_pixel_mapper_ = new_mapper;
  return true;
}

// -- Public interface of RGBMatrix. Delegate everything to impl_

RGBMatrix *RGBMatrix::CreateFromOptions(const RGBMatrix::Options &options, const RuntimeOptions &runtime_options) {

  // For the Pi4, we might need 2, maybe up to 4. Let's open up to 5.
  if (runtime_options.gpio_slowdown < 0 || runtime_options.gpio_slowdown > 5) {
    fprintf(stderr, "--led-slowdown-gpio=%d is outside usable range\n",
            runtime_options.gpio_slowdown);
    return NULL;
  }

  static GPIO io;  // This static var is a little bit icky.
  if (runtime_options.do_gpio_init && !io.Init(runtime_options.gpio_slowdown)) {
    return NULL;
  }

  RGBMatrix::Impl *result = new RGBMatrix::Impl(NULL, options);
  if (runtime_options.do_gpio_init)
    result->SetGPIO(&io, true);

  return new RGBMatrix(result);
}

FrameCanvas *RGBMatrix::CreateFrameCanvas() {
  return impl_->CreateFrameCanvas();
}
FrameCanvas *RGBMatrix::SwapOnVSync(FrameCanvas *other,
                                    unsigned framerate_fraction) {
  return impl_->SwapOnVSync(other, framerate_fraction);
}

bool RGBMatrix::StartRefresh() { return impl_->StartRefresh(); }
}  // end namespace rgb_matrix
