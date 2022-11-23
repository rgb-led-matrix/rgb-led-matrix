#include "Multiplex/SM5266Multiplex.h"
#include "port.h"

SM5266Multiplex::SM5266Multiplex(int rows) : Multiplex(rows) { }

void SM5266Multiplex::SetRow(int row) {
    gpioSet(c);
    for (int i = 7; i >= 0; i--) {
        if ((row % 8) == i)
            gpioSet(b);
        else
            gpioClear(b);
        gpioSet(a);
        gpioSet(a);
        gpioClear(a);
    }
    gpioClear(c);
    gpioWrite((row >> 3) % 4);
}

