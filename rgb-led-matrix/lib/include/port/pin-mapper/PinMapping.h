#ifndef PIN_MAPPING_H
#define PIN_MAPPING_H

#include "port/gpio/gpio-bits.h"

namespace rgb_matrix {
  class PinMapping {
    public:
      PinMapping() {}
      PinMapping(const char *s) : name(s) {}

      const char *name;
  };

  extern PinMapping *pin_mappings;
  extern uint32_t pin_mappings_size;
}

#endif
