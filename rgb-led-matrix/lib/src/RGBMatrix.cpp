#include "RGBMatrix.h"
#include "CFG/CFG.h"
#include "Panel/LEDPanel/LEDPanel.h"
#include "framebuffer/Framebuffer.h"
#include "framebuffer/external/external.h"

namespace rgb_matrix {
  Panel *RGBMatrix::CreateCanvas() {
    const MultiplexMapper *multiplex_mapper = NULL;

    if (_options.get_multiplexing() > 0) {
      const MuxMapperList &multiplexers = GetRegisteredMultiplexMappers();
      
      if (_options.get_multiplexing() <= (int) multiplexers.size())
        multiplex_mapper = multiplexers[_options.get_multiplexing() - 1];
    }

    if (multiplex_mapper) {
      multiplex_mapper->EditColsRows(&_options.get_cfg()->get_dot().cols, &_options.get_cfg()->get_dot().rows);
    }

    switch (_options.get_cfg()->get_id()) {
      case Canvas_ID::RP2040_ID:
        return new LEDPanel<PixelDesignator>(Framebuffer<PixelDesignator>::CreateFramebuffer(_options, multiplex_mapper));
      default:
        return nullptr;
    }
    
  }
};
