#include "port/pin-mapper/HUB75/HUB75_Pins.h"

namespace rgb_matrix {
  struct PinMapping HUB75_pin_mappings[] = {
    {
      // Terminates list
      HUB75_Pins((char *) "", -1, -1, HUB75_t(-1, -1, -1, -1, -1, -1) )
    },
  };
}
