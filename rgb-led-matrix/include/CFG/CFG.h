#ifndef CFG_H
#define CFG_H

#include "types.h"
#include <vector>
using namespace rgb_matrix;

namespace rgb_matrix {
  class CFG {
    public:
      CFG(int rows, int cols);

      Canvas_ID get_id() { return id_; }

      DOTCorrect dot;
      GAMMA gamma;
    
    protected:
      virtual bool isValid() = 0; 

      Canvas_ID id_;
  };
}

#endif