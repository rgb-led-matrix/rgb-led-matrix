#ifndef RGBMATRIX_H
#define RGBMATRIX_H

#include <stdint.h>
#include <stddef.h>
#include "canvas.h"

namespace rgb_matrix {

  struct GAMMA {
    GAMMA() : red(2.2), green(2.2), blue (2.2) {}

    float red;
    float green;
    float blue;
  };

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

  class CFG {
    public:
      CFG(int rows, int cols);

      DOTCorrect dot;
      GAMMA gamma;
    
    protected:
      virtual bool isValid() = 0; 
  };

  class RP2040_CFG : public CFG {
    public:
      RP2040_CFG(int rows, int cols) : CFG(rows, cols) {}

      int rows_;
      int cols_;
      int pwm_bits_;
      int brightness_;
    
    protected:
      bool isValid() { return true; }
  };

  class BCM_CFG : public CFG {
    public:
      BCM_CFG(int rows, int cols) : CFG(rows, cols) {}

      int pwm_bits_;
      int brightness_;
    
    protected:
      bool isValid() { return true; }
  };

  struct Options {
    Options(Canvas_ID id, CFG *cfg);

    Canvas_ID id;
    CFG *cfg;

    int multiplexing;
    const char *pixel_mapper_config;
    const char *hardware_mapping;
  };

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
