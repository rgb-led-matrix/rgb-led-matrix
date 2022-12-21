#ifndef BCM_CFG_H
#define BCM_CFG_H

#include "CFG/CFG.h"

namespace rgb_matrix {
  class HUB75_BCM_CFG : public CFG {
    public:
      HUB75_BCM_CFG(int rows, int cols) : CFG(rows, cols) { id_ = Canvas_ID::HUB75_BCM_ID;  }

      int thread_priority[2];
      uint32_t thread_affinity[2];

      int pwm_bits_;
      int brightness_;
      int chain_;
      int parallel_;
    
    protected:
      bool isValid() { return true; }
  };
}

#endif