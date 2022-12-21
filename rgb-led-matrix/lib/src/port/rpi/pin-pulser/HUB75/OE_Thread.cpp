#include "port/pin-pulser/HUB75/OE_Thread.h"

namespace rgb_matrix {
    template <typename T> OE_Thread<T>::OE_Thread(CFG *cfg, HUB75_Pins *pins) : pins_(pins) {
        // Do Nothing
    }

    template <typename T> Thread *OE_Thread<T>::CreateThread(CFG *cfg, HUB75_Pins *pins) {
        return nullptr;
    }

    template <typename T> void OE_Thread<T>::Run() {
        while (1) {
            // TODO: Show me the code!!!
        }
    }
}