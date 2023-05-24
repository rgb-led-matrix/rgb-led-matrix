#ifndef CFG_H
#define CFG_H

#include "types.h"
#include <vector>
using namespace rgb_matrix;

namespace rgb_matrix {
  class CFG {
    public:
      CFG(int rows, int cols) : dot(rows, cols) {}

      inline Canvas_ID get_id() { return id_; }
      inline DOTCorrect& get_dot() { return dot; }
      inline GAMMA& get_gamma() { return gamma; }
    
    protected:
      virtual bool isValid() = 0; 

      DOTCorrect dot;
      GAMMA gamma;
      Canvas_ID id_;
  };
}

#endif