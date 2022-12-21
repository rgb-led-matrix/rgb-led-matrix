#include "worker/BackgroundThread.h"
#include "port/pin-pulser/OE_Thread.h"

namespace rgb_matrix {
    Thread *BackgroundThread::ptr = nullptr;

    void BackgroundThread::CreateThread(CFG *cfg, PinMapping *pins) {
        switch (cfg->get_id()) {
            case Canvas_ID::BCM_ID:
                ptr = OE_Thread<int, int>::CreateThread(cfg, pins);
                break;
            case Canvas_ID::RP2040_Multiplexed_PMP_ID:
                // Do nothing
                break;
        }
    }

    void BackgroundThread::DestroyThread() {
        if (ptr != nullptr) {
            delete ptr;
            ptr = nullptr;
        }
    }
};