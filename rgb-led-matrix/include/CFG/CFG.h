#ifndef CFG_H
#define CFG_H

#include <vector>
#include "types.h"
#include "../IO/Node.h"
using namespace rgb_matrix;

namespace rgb_matrix {
  class CFG {
    public:
      CFG(int rows, int cols, Node *node, float red = 2.2, float green = 2.2, float blue = 2.2) : dot_(rows, cols), node_(node), gamma_(red, green, blue) {}

      inline Canvas_ID get_id() { return id_; }
      inline DOTCorrect& get_dot() { return dot_; }
      inline GAMMA& get_gamma() { return gamma_; }
    
    protected:
      virtual bool isValid() = 0; 

      DOTCorrect dot_;
      GAMMA gamma_;
      Canvas_ID id_;
      Node *node_;
  };
}

#endif