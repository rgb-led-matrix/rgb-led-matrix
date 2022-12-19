#ifndef RGBMATRIX_H
#define RGBMATRIX_H

#include <stdint.h>
#include <stddef.h>
#include "canvas.h"

namespace rgb_matrix {
  class RGBMatrix {
    public:
      virtual ~RGBMatrix();

      static RGBMatrix *CreateFromOptions(Options &options);

      virtual Canvas *CreateCanvas();
      virtual void show(Canvas *c);

    protected:
      RGBMatrix() : _options(Options(Canvas_ID::BCM_ID, nullptr)) {}
      RGBMatrix(Options o);

      Options _options;
      static RGBMatrix *_ptr;
  };
}
#endif
