#include "Multiplex/DirectABCDLineMultiplex.h"
#include "port.h"

DirectABCDLineMultiplex::DirectABCDLineMultiplex(int rows) : Multiplex(rows) { }

void DirectABCDLineMultiplex::SetRow(int row) {
    gpioWrite(1 << (row % 4));
}

