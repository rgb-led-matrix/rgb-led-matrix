#ifndef RGBMATRIX_H
#define RGBMATRIX_H

#include <stddef.h>
#include "Panel.h"
#include "CFG/types.h"

namespace rgb_matrix {
  class RGBMatrix {
    public:
      RGBMatrix(Options o) :_options(o) {}
      virtual ~RGBMatrix() {}

      virtual Panel *CreateCanvas();

    protected:
      RGBMatrix() : _options(Options(nullptr, 0, nullptr)) {}

      Options _options;
  };
}
#endif
