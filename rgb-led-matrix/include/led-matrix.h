#ifndef RGBMATRIX_H
#define RGBMATRIX_H

#include <stdint.h>
#include <stddef.h>
#include "canvas.h"

namespace rgb_matrix {

  struct DOTCorrect {
    public:
      DOTCorrect(int rows, int cols);
      ~DOTCorrect();

      bool set(int x, int y, float red, float green, float blue);
      void get(int x, int y, float *red, float *green, float *blue);

      // TODO: Limit access
      int rows;
      int cols;
    
    private:
      bool check(float f);

      float *table_;
  };

  struct Options {
    Options();

    const char *hardware_mapping;
    struct DOTCorrect dot;
    int pwm_bits;
    int brightness;
    int multiplexing;
    const char *pixel_mapper_config;
  };

  class RGBMatrix {
    public:
      virtual ~RGBMatrix();

      static RGBMatrix *CreateFromOptions(Options &options);

      virtual Canvas *CreateCanvas(Canvas_ID id);
      virtual void show(Canvas *c);

      // TODO: RGB Gamma, Brightness, Dot correction, CIE1931, etc.

    protected:
      RGBMatrix() {}
      RGBMatrix(Options o);

      Options _options;
      static RGBMatrix *_ptr;
    };
}
#endif
