#ifndef FT4232_H
#define FT4232_H

#include <stdint.h>
#include "Root.h"

namespace rgb_matrix {
  class FT4232 : public Root {
    public:
        FT4232();

        Node *GetNode(uint32_t index);
  };
}
#endif
