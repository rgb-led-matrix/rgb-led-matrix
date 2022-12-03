#include "port/pin-mapper/RP2040/RP2040_Pins.h"

#define GPIO_BIT(b) ((uint32_t) 1 << (b))

// This is that stuff which is not allowed in C++
#define TX(b)         GPIO_BIT(b)
#define RX(b)         GPIO_BIT(b)
#define reset(b)      GPIO_BIT(b)
#define bootloader(b) GPIO_BIT(b)
#define cs_out(b)     GPIO_BIT(b)
#define address0(b)   GPIO_BIT(b)
#define address1(b)   GPIO_BIT(b)
#define address2(b)   GPIO_BIT(b)
#define address3(b)   GPIO_BIT(b)


struct PinMapping pin_mappings[] = {
  {
    // Hopefully someday this will get fixed
    RP2040_Pins((char *) "regular", TX(18), RX(18), reset(18), bootloader(18),
      cs_out(18), address0(18), address1(18), address2(18), address3(18) )
  },

  {
    // Terminates list
    RP2040_Pins((char *) "", TX(18), RX(18), reset(18), bootloader(18),
      cs_out(18), address0(18), address1(18), address2(18), address3(18) )
  },
};
