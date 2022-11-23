#include "Multiplex/DirectMultiplex.h"
#include "port.h"

DirectMultiplex::DirectMultiplex(int rows) : Multiplex(rows) { }

void DirectMultiplex::SetRow(int row) {
    gpioWrite(row % _rows);
}

