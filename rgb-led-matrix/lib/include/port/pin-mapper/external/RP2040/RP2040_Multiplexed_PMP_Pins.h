#ifndef RP2040_PIN_MAPPING_H
#define RP2040_PIN_MAPPING_H

#include "port/pin-mapper/PinMapping.h"

namespace rgb_matrix {

  struct wr_t {
    wr_t(uint32_t wr0, uint32_t wr1, uint32_t wr2, uint32_t wr3) {
      wr[0] = 1 << wr0;
      wr[1] = 1 << wr1;
      wr[2] = 1 << wr2;
      wr[3] = 1 << wr3;
    }

    gpio_bits_t wr[4];
  };

  class RP2040_Multiplexed_PMP_Pins : public PinMapping {
    public:
      RP2040_Multiplexed_PMP_Pins(const char *name, uint32_t dat, wr_t wr, uint32_t reset) 
        : PinMapping(name), dat(1 << dat), wr(wr), reset(1 << reset) {}

      const wr_t wr;
      const uint32_t dat;         // Defines 8 pins (dat + 0, dat + 1, ..., dat + 7)
      const gpio_bits_t reset;    // Not used (This is the actual reset used to signal USB bootloader!)
  };

  extern RP2040_Multiplexed_PMP_Pins RP2040_Multiplexed_PMP_pin_mappings[];
  extern const uint32_t RP2040_Multiplexed_PMP_pin_mappings_size;
}

#endif
