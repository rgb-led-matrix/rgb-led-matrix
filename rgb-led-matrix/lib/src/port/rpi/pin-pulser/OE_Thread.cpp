#include "port/pin-pulser/OE_Thread.h"

namespace rgb_matrix {
    template <typename T, typename R> OE_Thread<T, R>::OE_Thread(T *pins) : pins_(pins) {

    }

    template <typename T, typename R> Thread *OE_Thread<T, R>::CreateThread(CFG *cfg, PinMapping *pins) {

    }

    template <typename T, typename R> void OE_Thread<T, R>::Run() {

    }
}