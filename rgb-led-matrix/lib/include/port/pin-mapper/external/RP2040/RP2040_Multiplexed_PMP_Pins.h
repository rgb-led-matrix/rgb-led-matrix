#ifndef RP2040_PIN_MAPPING_H
#define RP2040_PIN_MAPPING_H

#include "port/pin-mapper/PinMapping.h"

namespace rgb_matrix {

  struct cs_t {
    cs_t(uint32_t cs0, uint32_t cs1, uint32_t cs2, uint32_t cs3) {
      cs[0] = 1 << cs0;
      cs[1] = 1 << cs1;
      cs[2] = 1 << cs2;
      cs[3] = 1 << cs3;
    }

    gpio_bits_t cs[4];
  };

  struct RP2040_Multiplexed_PMP_Pins : public PinMapping {
    public:
      RP2040_Multiplexed_PMP_Pins(const char *name, uint32_t clk, uint32_t dat, cs_t cs, uint32_t reset) 
        : PinMapping(name), clk(clk), dat(1 << dat), cs(cs), reset(1 << reset) {}

      const cs_t cs;
      const gpio_bits_t clk;
      const uint32_t dat;       // Defines 8 pins (dat + 0, dat + 1, ..., dat + 7)
      const uint32_t reset;     // Not used
  };

  extern struct PinMapping RP2040_Multiplexed_PMP_pin_mappings[];
}

#endif
