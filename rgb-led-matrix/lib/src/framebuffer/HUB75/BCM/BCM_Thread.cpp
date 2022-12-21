#include "framebuffer/HUB75/BCM/BCM_Thread.h"
#include "CFG/HUB75_BCM_CFG.h"

namespace rgb_matrix {
    BCM_Thread::BCM_Thread(CFG *cfg) {

        HUB75_BCM_CFG *cfg_ptr = nullptr;
        if (cfg->get_id() == Canvas_ID::HUB75_BCM_ID)
            cfg_ptr = static_cast<HUB75_BCM_CFG *>(cfg);
        else
            throw cfg;

        Start(cfg_ptr->thread_priority[0], cfg_ptr->thread_affinity[0]);
    }

    BCM_Thread::~BCM_Thread() {
        // Do Nothing
    }

    void BCM_Thread::Run() {
        // TODO:
    }
};