#ifndef RGBMATRIX_H
#define RGBMATRIX_H

#include <stddef.h>
#include "Panel/Panel.h"
#include "CFG/types.h"

namespace rgb_matrix {
  class RGBMatrix {
    public:
      RGBMatrix(CFG *cfg) :_cfg(cfg) {}
      virtual ~RGBMatrix() {}

      virtual Panel *CreateCanvas();

    protected:
      RGBMatrix() : _cfg(nullptr) {}

      CFG *_cfg;
  };
}
#endif
