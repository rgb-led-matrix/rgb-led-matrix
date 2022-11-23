#include "Multiplex/ShiftRegisterMultiplex.h"
#include "port.h"

ShiftRegisterMultiplex::ShiftRegisterMultiplex(int rows) : Multiplex(rows) { }

void ShiftRegisterMultiplex::SetRow(int row) {
    for (int i = 0; i < _rows; ++i) {
        gpioClear(a);
        if (i == (_rows - row - 1))
            gpioClear(b);
        else
            gpioSet(b);
        gpioSet(a);
    }
    gpioClear(a);
    gpioSet(a);
}

