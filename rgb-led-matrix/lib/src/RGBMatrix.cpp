#include "RGBMatrix.h"
#include "CFG/CFG.h"
#include "framebuffer/Framebuffer.h"
#include "framebuffer/RGB/RGB24.h"
#include "framebuffer/RGB/RGB48.h"
#include "framebuffer/RGB/RGB_222.h"
#include "framebuffer/RGB/RGB_555.h"

namespace rgb_matrix {
  Panel *RGBMatrix::CreatePanel() {
    switch (_cfg->get_data_format()) {
      case Data_Format_ID::RGB48_ID:
        return Framebuffer<RGB48>::CreateFramebuffer(_cfg);
      case Data_Format_ID::RGB24_ID:
        return Framebuffer<RGB24>::CreateFramebuffer(_cfg);
      case Data_Format_ID::RGB_222_ID:
        return Framebuffer<RGB_222>::CreateFramebuffer(_cfg);
      case Data_Format_ID::RGB_555_ID:
        return Framebuffer<RGB_555>::CreateFramebuffer(_cfg);
      default:
        return nullptr;
    }
  }
};
