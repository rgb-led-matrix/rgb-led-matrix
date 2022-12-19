#include "port/pin-mapper/external/RP2040/RP2040_Multiplexed_PMP_Pins.h"

namespace rgb_matrix {
  RP2040_Multiplexed_PMP_Pins RP2040_Multiplexed_PMP_pin_mappings[] = {
    {
      RP2040_Multiplexed_PMP_Pins("regular", 4, wr_t(24, 25, 26, 27), 23)
    }
  };

  const uint32_t RP2040_Multiplexed_PMP_pin_mappings_size = sizeof(RP2040_Multiplexed_PMP_pin_mappings) / sizeof(RP2040_Multiplexed_PMP_Pins);
}
