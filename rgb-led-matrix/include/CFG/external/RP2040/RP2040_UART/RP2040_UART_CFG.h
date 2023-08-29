#ifndef RP2040_UART_CFG_H
#define RP2040_UART_CFG_H

#include "CFG/CFG.h"

namespace rgb_matrix {
  class RP2040_UART_CFG : public CFG {
    public:
      RP2040_UART_CFG(int rows, int cols, Node *node, Data_Format_ID data_format, Panel_Type type, uint8_t scan, int pwm_bits) 
        : CFG(rows, cols, node, data_format, type, scan), pwm_bits_(pwm_bits) { 
            id_ = External_ID::RP2040_UART_ID; 
      }

      inline int get_pwm_bits() { return pwm_bits_; }
    
    protected:
      inline bool isValid() {  return true; }

      int pwm_bits_;
  };
}

#endif