#ifndef RP2040_PIN_MAPPING_H
#define RP2040_PIN_MAPPING_H

#include "port/pin-mapper/PinMapping.h"

namespace rgb_matrix {

  struct wr_t {
    wr_t(gpio_bits_t wr0, gpio_bits_t wr1, gpio_bits_t wr2, gpio_bits_t wr3)
      : wr{(gpio_bits_t) 1 << wr0, (gpio_bits_t) 1 << wr1, (gpio_bits_t) 1 << wr2, (gpio_bits_t) 1 << wr3} {}

    const gpio_bits_t wr[4];
  };

  struct RP2040_Multiplexed_PMP_Pins : public PinMapping {
    public:
      RP2040_Multiplexed_PMP_Pins(const char *name, gpio_bits_t dat, wr_t wr, gpio_bits_t reset) 
        : PinMapping(name), dat(1 << dat), wr(wr), reset(1 << reset) {}

      const wr_t wr;
      const gpio_bits_t dat;         // Defines 8 pins (dat + 0, dat + 1, ..., dat + 7)
      const gpio_bits_t reset;    // Not used (This is the actual reset used to signal USB bootloader!)
  };

  extern RP2040_Multiplexed_PMP_Pins RP2040_Multiplexed_PMP_pin_mappings[];
  extern const uint32_t RP2040_Multiplexed_PMP_pin_mappings_size;
}

#endif
