#include "port/pin-mapper/RP2040/RP2040_Pins.h"

#define GPIO_BIT(b) ((uint32_t) 1 << (b))

struct RP2040_Pins pin_mappings[] = {
  {
    RP2040_Pins((char *) "regular", GPIO_BIT(18), GPIO_BIT(18), GPIO_BIT(18), GPIO_BIT(18),
      GPIO_BIT(18), GPIO_BIT(18), GPIO_BIT(18), GPIO_BIT(18), GPIO_BIT(18) )
  },
};
