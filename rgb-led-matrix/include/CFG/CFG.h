#ifndef CFG_H
#define CFG_H

#include "types.h"
using namespace rgb_matrix;

namespace rgb_matrix {
  class CFG {
    public:
      CFG(int rows, int cols);

      DOTCorrect dot;
      GAMMA gamma;
    
    protected:
      virtual bool isValid() = 0; 

      friend struct Options;
      Canvas_ID id_;
  };
}

#endif