#include "pin-pulser/PinPulser.h"

namespace rgb_matrix {
    PinPulser::PinPulser(GPIO *io, HUB75_Pins *pins) : io(io), pins(pins) {
        // Do nothing
    }
}