#include "port/pin-mapper/external/RP2040/RP2040_Multiplexed_Dual_UART_Pins.h"

namespace rgb_matrix {
  struct PinMapping RP2040_Multiplexed_Dual_UART_pin_mappings[] = {
    {
      // Terminates list
      RP2040_Multiplexed_Dual_UART_Pins("", "", "", cs_t(-1, -1, -1, -1))
    },
  };
}