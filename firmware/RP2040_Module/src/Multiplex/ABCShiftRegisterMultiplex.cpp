#include "Multiplex/ABCShiftRegisterMultiplex.h"
#include "port.h"

ABCShiftRegisterMultiplex::ABCShiftRegisterMultiplex(int rows) : Multiplex(rows) { }

void ABCShiftRegisterMultiplex::SetRow(int row) {
    for (int i = 0; i < _rows; ++i) {
        gpioClear(a);
        if (i == (_rows - row - 1))
            gpioSet(c);
        else
            gpioClear(c);
        gpioSet(a);
    }
    gpioSet(a);
    gpioClear(a);
}

