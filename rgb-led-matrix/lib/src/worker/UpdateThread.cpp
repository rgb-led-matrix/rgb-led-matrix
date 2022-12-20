#include "worker/UpdateThread.h"

namespace rgb_matrix {
    Thread *UpdateThread::ptr = nullptr;

    void UpdateThread::CreateThread(CFG *cfg) {
        switch (cfg->get_id()) {
            case Canvas_ID::BCM_ID:
                // TODO: Create BCM_Thread, assign to ptr
                break;
            case Canvas_ID::RP2040_Multiplexed_PMP_ID:
                // Do nothing
                break;
        }
    }

    void UpdateThread::DestroyThread() {
        if (ptr != nullptr) {
            delete ptr;
            ptr = nullptr;
        }
    }
};