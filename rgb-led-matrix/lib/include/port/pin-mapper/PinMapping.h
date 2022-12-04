#ifndef PIN_MAPPING_H
#define PIN_MAPPING_H

#include "port/gpio/gpio-bits.h"

struct PinMapping {
  public:
    PinMapping(char *s) : name(s) {}

    const char *name;
};

extern struct PinMapping **pin_mappings;

#endif
