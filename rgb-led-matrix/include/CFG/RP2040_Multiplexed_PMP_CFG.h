#ifndef RP2040_CFG_H
#define RP2040_CFG_H

#include "CFG/CFG.h"

namespace rgb_matrix {
  class RP2040_Multiplexed_PMP_CFG : public CFG {
    public:
      RP2040_Multiplexed_PMP_CFG(int rows, int cols) : CFG(rows, cols) { id_ = Canvas_ID::RP2040_Multiplexed_PMP_ID; }

      int rows_;
      int cols_;
      int pwm_bits_;
      int brightness_;
    
    protected:
      bool isValid() { return true; }

  };
}

#endif