#include "worker/UpdateThread.h"
#include "framebuffer/HUB75/BCM/BCM_Thread.h"

namespace rgb_matrix {
    Thread *UpdateThread::ptr = nullptr;

    void UpdateThread::CreateThread(CFG *cfg) {
        switch (cfg->get_id()) {
            case Canvas_ID::HUB75_BCM_ID:
                ptr = new BCM_Thread(cfg);
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