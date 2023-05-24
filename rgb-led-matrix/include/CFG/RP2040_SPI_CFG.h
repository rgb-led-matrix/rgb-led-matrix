#ifndef RP2040_CFG_H
#define RP2040_CFG_H

#include "CFG/CFG.h"

namespace rgb_matrix {
  class RP2040_SPI_CFG : public CFG {
    public:
      RP2040_SPI_CFG(int rows, int cols, float red, float green, float blue, int pwm_bits, int brightness, bool use_CIE1931, 
        bool use_gamma_correction, bool use_dot_correction, bool use_brightness, const char *spidev_path) : 
          CFG(rows, cols, red, green, blue), spidev_path_(spidev_path), pwm_bits_(pwm_bits), brightness_(brightness), use_CIE1931_(use_CIE1931), 
          use_gamma_correction_(use_gamma_correction), use_dot_correction_(use_dot_correction), use_brightness_(use_brightness) { 
            id_ = Canvas_ID::RP2040_SPI_ID; 
      }

      inline bool use_brightness() { return use_brightness_; }
      inline bool use_gamma_correction() { return use_gamma_correction_; }
      inline bool use_CIE1931() { return use_CIE1931_; }
      inline bool use_dot_correction() { return use_dot_correction_; }
      inline int get_pwm_bits() { return pwm_bits_; }
      inline int get_brightness() { return brightness_; }
      inline const char *get_spidev_path() { return spidev_path_; }
    
    protected:
      inline bool isValid() {  return true; }

      int pwm_bits_;
      int brightness_;
      bool use_gamma_correction_;
      bool use_CIE1931_;
      bool use_dot_correction_;
      bool use_brightness_;
      const char *spidev_path_;

  };
}

#endif