#ifndef PIN_MAPPING_H
#define PIN_MAPPING_H

#include "port/gpio/gpio-bits.h"

namespace rgb_matrix {
  struct PinMapping {
    public:
      PinMapping(const char *s) : name(s) {}

      const char *name;
  };

  extern struct PinMapping **pin_mappings;
}

#endif
