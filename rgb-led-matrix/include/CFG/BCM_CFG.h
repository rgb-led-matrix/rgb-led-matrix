#ifndef BCM_CFG_H
#define BCM_CFG_H

#include "CFG/CFG.h"

namespace rgb_matrix {
  class BCM_CFG : public CFG {
    public:
      BCM_CFG(int rows, int cols) : CFG(rows, cols) { id_ = Canvas_ID::BCM_ID;  }

      int pwm_bits_;
      int brightness_;
    
    protected:
      bool isValid() { return true; }
  };
}

#endif