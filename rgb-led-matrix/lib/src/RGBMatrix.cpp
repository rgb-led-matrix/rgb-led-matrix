#include "RGBMatrix.h"
#include "CFG/CFG.h"
#include "Panel/LEDPanel/LEDPanel.h"
#include "framebuffer/Framebuffer.h"
#include "framebuffer/external/external.h"

namespace rgb_matrix {
  Panel *RGBMatrix::CreateCanvas() {
    switch (_cfg->get_id()) {
      case Canvas_ID::RP2040_UART_ID:
        return new LEDPanel<PixelDesignator>(Framebuffer<PixelDesignator>::CreateFramebuffer(_cfg));
      default:
        return nullptr;
    }
  }
};
