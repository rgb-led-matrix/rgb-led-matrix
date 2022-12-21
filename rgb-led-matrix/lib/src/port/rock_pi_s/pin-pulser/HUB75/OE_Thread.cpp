#include "port/pin-pulser/HUB75/OE_Thread.h"
#include "CFG/BCM_CFG.h"

namespace rgb_matrix {
    template <typename T> OE_Thread<T>::OE_Thread(CFG *cfg, HUB75_Pins *pins) : pins_(pins) {
        io_ = new GPIO(); // TODO: Convert to singleton
        
        switch (cfg->get_id()) {
            case Canvas_ID::BCM_ID:
                BCM_CFG *p = static_cast<BCM_CFG *>(cfg);
                Start(p->thread_priority[1], p->thread_affinity[1]);
                break;
            default:
                throw cfg->get_id();
                break;
        }
    }

    template <typename T> Thread *OE_Thread<T>::CreateThread(CFG *cfg, HUB75_Pins *pins) {
        switch (cfg->get_id()) {
            case Canvas_ID::BCM_ID:
                // TODO: Switch on RowAddressSetter
                //return new OE_Thread<Direct>(cfg, pins);
                break;
            default:
                throw cfg->get_id();
                break;
        }
    }

    template <typename T> void OE_Thread<T>::Run() {
        while (1) {
            // TODO
        }
    }
}