#include "Multiplex/DirectMultiplex.h"
#include "Multiplex/ShiftRegisterMultiplex.h"
#include "Multiplex/DirectABCDLineMultiplex.h"
#include "Multiplex/ABCShiftRegisterMultiplex.h"
#include "Multiplex/SM5266Multiplex.h"

Multiplex::Multiplex(int rows) {
    _rows = rows;
}

Multiplex::~Multiplex() {
    // Do nothing
}

Multiplex *Multiplex::getMultiplexer(int rows, int num) {
    switch (num) {
        case 0:
            return new DirectMultiplex(rows);
        case 1:
            return new ShiftRegisterMultiplex(rows);
        case 2:
            return new DirectABCDLineMultiplex(rows);
        case 3:
            return new ABCShiftRegisterMultiplex(rows);
        case 4:
            return new SM5266Multiplex(rows);
        default:
            return nullptr;
    }
}
