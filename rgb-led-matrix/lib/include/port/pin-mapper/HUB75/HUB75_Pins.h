#ifndef HUB75_PIN_MAPPING_H
#define HUB75_PIN_MAPPING_H

#include "port/pin-mapper/PinMapping.h"

namespace rgb_matrix {
  struct HUB75_t {
    HUB75_t(uint32_t r0, uint32_t g0, uint32_t b0, uint32_t r1, uint32_t g1, uint32_t b1)
      : r0(1 << r0), g0(1 << g0), b0(1 << b0), r1(1 << r1), b1(1 << b1), g1(1 << g1) {}

    const gpio_bits_t r0;
    const gpio_bits_t g0;
    const gpio_bits_t b0;
    const gpio_bits_t r1;
    const gpio_bits_t g1;
    const gpio_bits_t b1;
  };

  struct HUB75_Pins : public PinMapping {
    public:
      HUB75_Pins(const char *name, uint32_t clk, uint32_t lat, HUB75_t p)
          : PinMapping(name), clk(1 << clk), lat(1 << lat), num(1), p0(p), p1(p), p2(p) {}

      HUB75_Pins(const char *name, uint32_t clk, uint32_t lat, HUB75_t p0, HUB75_t p1)
          : PinMapping(name), clk(1 << clk), lat(1 << lat), num(2), p0(p0), p1(p1), p2(p0) {}

      HUB75_Pins(const char *name, uint32_t clk, uint32_t lat, HUB75_t p0, HUB75_t p1, HUB75_t p2)
          : PinMapping(name), clk(1 << clk), lat(1 << lat), num(3), p0(p0), p1(p1), p2(p2) {}


      const gpio_bits_t clk;
      const gpio_bits_t lat;
      const HUB75_t p0;
      const HUB75_t p1;
      const HUB75_t p2;
      const uint8_t num;
  };

  extern struct PinMapping HUB75_pin_mappings[];
}

#endif
