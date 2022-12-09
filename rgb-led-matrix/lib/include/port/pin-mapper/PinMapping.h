#ifndef PIN_MAPPING_H
#define PIN_MAPPING_H

#include <list>
#include "port/gpio/gpio-bits.h"

namespace rgb_matrix {
  class PinMapping {
    public:
      PinMapping(const char *s) : name(s) {}

      const char *name;
  };

  extern std::list<PinMapping> *pin_mappings;
}

#endif
