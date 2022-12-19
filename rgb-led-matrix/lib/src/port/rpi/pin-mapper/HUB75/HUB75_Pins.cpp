#include "port/pin-mapper/HUB75/HUB75_Pins.h"

namespace rgb_matrix {
  HUB75_Pins HUB75_pin_mappings[] = {
    {
      // Hopefully someday this will get fixed
      HUB75_Pins("regular", -1, -1, HUB75_t(-1, -1, -1, -1, -1, -1), HUB75_t(-1, -1, -1, -1, -1, -1), HUB75_t(-1, -1, -1, -1, -1, -1), -1, -1, -1 )
    },

    {
      // Hopefully someday this will get fixed
      HUB75_Pins("single", -1, -1, HUB75_t(-1, -1, -1, -1, -1, -1), -1, -1, -1 )
    },

    {
      // Hopefully someday this will get fixed
      HUB75_Pins("dual", -1, -1, HUB75_t(-1, -1, -1, -1, -1, -1), HUB75_t(-1, -1, -1, -1, -1, -1), -1, -1, -1 )
    }
  };

  const uint32_t HUB75_pin_mappings_size = sizeof(HUB75_pin_mappings) / sizeof(HUB75_Pins);
}
