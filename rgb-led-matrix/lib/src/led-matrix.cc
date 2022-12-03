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

#include "led-matrix.h"
#include "framecanvas.h"
#include "framebuffer/framebuffer.h"

namespace rgb_matrix {
RGBMatrix *RGBMatrix::_ptr = nullptr;

// TODO: Get rid of this!!!
using namespace internal;

Options::Options() :
  hardware_mapping("regular"),
  rows(32),
  cols(32),
  pwm_bits(Framebuffer::kDefaultBitPlanes),
  brightness(100),
  multiplexing(0),
  pixel_mapper_config(NULL)
{
  // Do nothing
}

RGBMatrix::RGBMatrix(Options o) :_options(o) {
  Framebuffer::InitHardwareMapping(_options.hardware_mapping);
}

RGBMatrix *RGBMatrix::CreateFromOptions(Options &options) {
  if (_ptr == nullptr) {
    _ptr = new RGBMatrix(options);
  }

  return _ptr;
}

Canvas *RGBMatrix::CreateCanvas(Canvas_ID id) {
  Framebuffer *buf = Framebuffer::CreateFramebuffer(id, _options.rows, _options.cols);

  if (buf == nullptr)
    return nullptr;

  const MultiplexMapper *multiplex_mapper = NULL;

  if (_options.multiplexing > 0) {
    const MuxMapperList &multiplexers = GetRegisteredMultiplexMappers();
    
    if (_options.multiplexing <= (int) multiplexers.size())
      multiplex_mapper = multiplexers[_options.multiplexing - 1];
  }

  if (multiplex_mapper)
    multiplex_mapper->EditColsRows(&_options.cols, &_options.rows);
  
  buf->InitSharedMapper(multiplex_mapper, _options.pixel_mapper_config);
  buf->SetBrightness(_options.brightness);
  buf->SetPWMBits(_options.pwm_bits);

  return new FrameCanvas(buf);
}

void RGBMatrix::show(Canvas *c) {
  c->show();
}
};
