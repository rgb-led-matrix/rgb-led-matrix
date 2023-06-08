#ifndef ROOT_H
#define ROOT_H

#include <stdint.h>
#include "IO/Node/Node.h"

namespace rgb_matrix {
  class Root {
    public:
      virtual ~Root() {}

      virtual Node *GetNode(uint32_t index) = 0;
  };
}
#endif
