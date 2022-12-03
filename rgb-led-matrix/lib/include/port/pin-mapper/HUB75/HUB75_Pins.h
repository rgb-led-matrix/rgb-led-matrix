#ifndef HUB75_PIN_MAPPING_H
#define HUB75_PIN_MAPPING_H

#include "port/pin-mapper/PinMapping.h"

struct HUB75_Pins : public PinMapping {
  public:
    HUB75_Pins(char *name, gpio_bits_t TX, gpio_bits_t RX, gpio_bits_t reset,
      gpio_bits_t bootloader, gpio_bits_t cs_out, gpio_bits_t address0,
      gpio_bits_t address1, gpio_bits_t address2, gpio_bits_t address3)
        : PinMapping(name), TX(TX), RX(RX), reset(reset), bootloader(bootloader),
          cs_out(cs_out), address0(address0), address1(address1),
          address2(address2), address3(address3) {}


    const gpio_bits_t TX;
    const gpio_bits_t RX;
    const gpio_bits_t reset;
    const gpio_bits_t bootloader;
    //const gpio_bits_t cs_in;
    const gpio_bits_t cs_out;
    const gpio_bits_t address0;
    const gpio_bits_t address1;
    const gpio_bits_t address2;
    const gpio_bits_t address3;
};

extern struct PinMapping HUB75_pin_mappings[];

#endif
