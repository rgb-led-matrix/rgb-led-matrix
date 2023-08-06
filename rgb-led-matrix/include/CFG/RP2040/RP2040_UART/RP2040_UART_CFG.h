#ifndef RP2040_UART_CFG_H
#define RP2040_UART_CFG_H

#include "CFG/CFG.h"

namespace rgb_matrix {
  class RP2040_UART_CFG : public CFG {
    public:
      RP2040_UART_CFG(int rows, int cols, Node *node, Panel_Type type, uint8_t scan, int pwm_bits, bool use_CIE1931) 
        : CFG(rows, cols, node, type, scan), pwm_bits_(pwm_bits), use_CIE1931_(use_CIE1931) { 
            id_ = External_ID::RP2040_UART_RGB48_ID; 
      }

      inline bool use_CIE1931() { return use_CIE1931_; }
      inline int get_pwm_bits() { return pwm_bits_; }
    
    protected:
      inline bool isValid() {  return true; }

      int pwm_bits_;
      bool use_CIE1931_;

  };
}

#endif