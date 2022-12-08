#include "port/pin-mapper/external/RP2040/RP2040_Multiplexed_PMP_Pins.h"

namespace rgb_matrix {
  struct PinMapping RP2040_Multiplexed_PMP_pin_mappings[] = {
    {
      RP2040_Multiplexed_PMP_Pins("regular", 4, wr_t(24, 25, 26, 27), 23)
    },

    {
      // Terminates list
      RP2040_Multiplexed_PMP_Pins("", -1, wr_t(-1, -1, -1, -1), -1)
    },
  };
}
