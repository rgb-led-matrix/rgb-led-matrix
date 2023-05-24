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

#include "RGBMatrix.h"
#include "CFG/CFG.h"
#include "LEDPanel.h"
#include "framebuffer/framebuffer.h"

namespace rgb_matrix {
  Panel *RGBMatrix::CreateCanvas() {
    const MultiplexMapper *multiplex_mapper = NULL;

    if (_options.get_multiplexing() > 0) {
      const MuxMapperList &multiplexers = GetRegisteredMultiplexMappers();
      
      if (_options.get_multiplexing() <= (int) multiplexers.size())
        multiplex_mapper = multiplexers[_options.get_multiplexing() - 1];
    }

    if (multiplex_mapper)
      multiplex_mapper->EditColsRows(&_options.get_cfg()->get_dot().cols, &_options.get_cfg()->get_dot().rows);

    switch (_options.get_cfg()->get_id()) {
      case Canvas_ID::RP2040_SPI_ID:
        return new LEDPanel<PixelDesignator>(Framebuffer<PixelDesignator>::CreateFramebuffer(_options, multiplex_mapper));
      default:
        return nullptr;
    }
    
  }
};
