#ifndef PIN_PULSER_H
#define PIN_PULSER_H

#include "port/gpio/gpio.h"

namespace rgb_matrix {

// A PinPulser is a utility class that pulses a GPIO pin. There can be various implementations.
class PinPulser {
public:
  static PinPulser *Create(GPIO *io, gpio_bits_t gpio_mask_trigger, gpio_bits_t gpio_mask_response, int refresh, int row_address_type);
  virtual ~PinPulser() {}

  // Send a pulse with a given length (index into nano_wait_spec array).
  virtual void SendPulse() = 0;

  // If SendPulse() is asynchronously implemented, wait for pulse to finish.
  virtual void WaitPulseFinished() {}
};

}

#endif