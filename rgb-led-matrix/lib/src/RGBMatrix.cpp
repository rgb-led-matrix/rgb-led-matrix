#include "RGBMatrix.h"
#include "CFG/CFG.h"
#include "framebuffer/Framebuffer.h"
#include "framebuffer/external/external.h"

namespace rgb_matrix {
  Panel *RGBMatrix::CreatePanel() {
    switch (_cfg->get_id()) {
      case External_ID::RP2040_UART_RGB48_ID:
        return Framebuffer<RGB48>::CreateFramebuffer(_cfg);
      case External_ID::RP2040_UART_RGB24_ID:
        return Framebuffer<RGB24>::CreateFramebuffer(_cfg);
      default:
        return nullptr;
    }
  }
};
