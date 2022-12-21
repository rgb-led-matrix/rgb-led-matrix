#include "framebuffer/HUB75/BCM/BCM_Thread.h"
#include "CFG/BCM_CFG.h"

namespace rgb_matrix {
    BCM_Thread::BCM_Thread(CFG *cfg) {

        BCM_CFG *cfg_ptr = nullptr;
        if (cfg->get_id() == Canvas_ID::BCM_ID)
            cfg_ptr = static_cast<BCM_CFG *>(cfg);
        else
            throw cfg;

        Start(cfg_ptr->thread_priority[0], cfg_ptr->thread_affinity[0]);
    }

    BCM_Thread::~BCM_Thread() {
        // Do Nothing
    }

    void BCM_Thread::Run() {

    }
};