#include "port/pin-mapper/external/RP2040/RP2040_Multiplexed_PMP_Pins.h"

namespace rgb_matrix {
  RP2040_Multiplexed_PMP_Pins RP2040_Multiplexed_PMP_pin_mappings[] = {
    {
      // Hopefully someday this will get fixed
      RP2040_Multiplexed_PMP_Pins("regular", 8, wr_t(16, 17, 18, 19), 20)
    }
  };

  const uint32_t RP2040_Multiplexed_PMP_pin_mappings_size = sizeof(RP2040_Multiplexed_PMP_pin_mappings) / sizeof(RP2040_Multiplexed_PMP_Pins);
}
