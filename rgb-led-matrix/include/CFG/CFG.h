#ifndef CFG_H
#define CFG_H

#include "types.h"
#include <vector>
using namespace rgb_matrix;

namespace rgb_matrix {
  class CFG {
    public:
      CFG(int rows, int cols, float red = 2.2, float green = 2.2, float blue = 2.2) : dot_(rows, cols), gamma_(red, green, blue) {}

      inline Canvas_ID get_id() { return id_; }
      inline DOTCorrect& get_dot() { return dot_; }
      inline GAMMA& get_gamma() { return gamma_; }
    
    protected:
      virtual bool isValid() = 0; 

      DOTCorrect dot_;
      GAMMA gamma_;
      Canvas_ID id_;
  };
}

#endif