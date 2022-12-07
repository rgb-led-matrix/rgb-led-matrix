#include "port/pin-mapper/external/RP2040/RP2040_Multiplexed_PMP_Pins.h"

namespace rgb_matrix {
  struct PinMapping RP2040_Multiplexed_PMP_pin_mappings[] = {
    {
      // Hopefully someday this will get fixed
      RP2040_Multiplexed_PMP_Pins("regular", 7, 8, cs_t(16, 17, 18, 19))
    },

    {
      // Terminates list
      RP2040_Multiplexed_PMP_Pins("", -1, -1, cs_t(-1, -1, -1, -1))
    },
  };
}
