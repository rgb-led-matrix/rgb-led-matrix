#include "RGBMatrix.h"
#include "CFG/CFG.h"
#include "Panel/LEDPanel/LEDPanel.h"
#include "framebuffer/Framebuffer.h"
#include "framebuffer/external/external.h"

namespace rgb_matrix {
  Panel *RGBMatrix::CreatePanel() {
    switch (_cfg->get_id()) {
      case Panel_ID::RP2040_UART_ID:
        return new LEDPanel<RGB48>(Framebuffer<RGB48>::CreateFramebuffer(_cfg));
      default:
        return nullptr;
    }
  }
};
