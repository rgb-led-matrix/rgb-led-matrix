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

    uint32_t cs[4];
  };

  struct RP2040_Multiplexed_Dual_UART_Pins : public PinMapping {
    public:
      RP2040_Multiplexed_Dual_UART_Pins(const char *name, const char *port0, const char *port1, cs_t cs) 
        : PinMapping(name), port0(port0), port1(port1), cs(cs) {}

      const cs_t cs;
      const char *port0;
      const char *port1;
  };

  extern struct PinMapping RP2040_Multiplexed_Dual_UART_pin_mappings[];
}

#endif
