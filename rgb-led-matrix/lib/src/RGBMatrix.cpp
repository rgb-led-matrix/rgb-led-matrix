#include "RGBMatrix.h"
#include "framebuffer/Framebuffer.h"
#include "framebuffer/external/external.h"

namespace rgb_matrix {
  Panel *RGBMatrix::CreatePanel() {
    return Framebuffer<RGB48>::CreateFramebuffer(_cfg);
  }
};
