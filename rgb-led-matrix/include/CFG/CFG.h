#ifndef CFG_H
#define CFG_H

#include "types.h"
#include <vector>
using namespace rgb_matrix;

namespace rgb_matrix {
  class CFG {
    public:
      CFG(int rows, int cols, int chains, float red = 2.2, float green = 2.2, float blue = 2.2) : dot_(rows, cols), chains_(chains), gamma_(red, green, blue) {}

      inline Canvas_ID get_id() { return id_; }
      inline DOTCorrect& get_dot() { return dot_; }
      inline GAMMA& get_gamma() { return gamma_; }
      inline int get_chains() { return chains_; }
    
    protected:
      virtual bool isValid() = 0; 

      int chains_;
      DOTCorrect dot_;
      GAMMA gamma_;
      Canvas_ID id_;
  };
}

#endif