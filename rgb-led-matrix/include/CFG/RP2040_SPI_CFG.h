#ifndef RP2040_CFG_H
#define RP2040_CFG_H

#include "CFG/CFG.h"

namespace rgb_matrix {
  class RP2040_SPI_CFG : public CFG {
    public:
      RP2040_SPI_CFG(int rows, int cols, const char *spidev_path) : CFG(rows, cols), spidev_path_(spidev_path) { 
        id_ = Canvas_ID::RP2040_SPI_ID; 
        use_CIE1931 = true;
        use_gamma_correction = true;
        use_dot_correction = true;
        use_brightness = true;
      }

      int rows_;
      int cols_;
      int pwm_bits_;
      int brightness_;

      bool use_gamma_correction;
      bool use_CIE1931;
      bool use_dot_correction;
      bool use_brightness;

      const char *spidev_path_;
    
    protected:
      bool isValid() { return true; }

  };
}

#endif