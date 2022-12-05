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
#include "CFG/CFG.h"
#include "framecanvas.h"
#include "framebuffer/framebuffer.h"

namespace rgb_matrix {
  RGBMatrix *RGBMatrix::_ptr = nullptr;

  // TODO: Get rid of this!!!
  using namespace internal;

  CFG::CFG(int rows, int cols) :
    dot(rows, cols) 
  {
    // Do nothing
  }

  Options::Options(Canvas_ID canvas_id, CFG *config) :
    hardware_mapping("regular"),
    cfg(config),
    id(canvas_id),
    multiplexing(0),
    pixel_mapper_config(NULL)
  {
    assert(canvas_id == config->id_);
  }

  DOTCorrect::DOTCorrect(int r, int c) : rows(r), cols(c) {
    table_ = new float[3 * rows * cols];

    for (int y = 0; y < rows; ++y) {
      for (int x = 0; x < cols; ++x) {
        table_[3 * (y * cols + x)] = 1.0;
        table_[3 * (y * cols + x) + 1] = 1.0;
        table_[3 * (y * cols + x) + 2] = 1.0;
      }
    }
  }

  DOTCorrect::~DOTCorrect() {
    delete table_;
  }

  bool DOTCorrect::set(int x, int y, float red, float green, float blue) {
    if (check(red) || check(green) || check(blue))
      return false;

    table_[y * cols + x] = red;
    table_[y * cols + x + 1] = green;
    table_[y * cols + x + 2] = blue;

    return true;
  }

  void DOTCorrect::get(int x, int y, float *red, float *green, float *blue) {
    *red = table_[y * cols + x];
    *green = table_[y * cols + x + 1];
    *blue = table_[y * cols + x + 2];
  }

  bool DOTCorrect::check(float f) {
    return !(f < 0.5 || f > 1.0);
  }

  RGBMatrix::RGBMatrix(Options o) :_options(o) {
    Framebuffer<PixelDesignator>::InitHardwareMapping(_options.hardware_mapping);
  }

  RGBMatrix::~RGBMatrix() {
    if (_ptr != nullptr)
      delete _ptr;
    _ptr = nullptr;
  }

  RGBMatrix *RGBMatrix::CreateFromOptions(Options &options) {
    if (_ptr == nullptr) {
      _ptr = new RGBMatrix(options);
    }

    return _ptr;
  }

  Canvas *RGBMatrix::CreateCanvas() {
    const MultiplexMapper *multiplex_mapper = NULL;

    if (_options.multiplexing > 0) {
      const MuxMapperList &multiplexers = GetRegisteredMultiplexMappers();
      
      if (_options.multiplexing <= (int) multiplexers.size())
        multiplex_mapper = multiplexers[_options.multiplexing - 1];
    }

    if (multiplex_mapper)
      multiplex_mapper->EditColsRows(&_options.cfg->dot.cols, &_options.cfg->dot.rows);

    switch (_options.id) {
      case Canvas_ID::RP2040_ID:
        return new FrameCanvas<PixelDesignator>(Framebuffer<PixelDesignator>::CreateFramebuffer(_options, multiplex_mapper));
      case Canvas_ID::BCM_ID:
        return new FrameCanvas<PixelDesignator_HUB75>(Framebuffer<PixelDesignator_HUB75>::CreateFramebuffer(_options, multiplex_mapper));
      default:
        return nullptr;
    }
    
  }

  void RGBMatrix::show(Canvas *c) {
    c->show();
  }
};
