#include <math.h>
#include "framebuffer/external/RP2040/RP2040_UART/RP2040_UART.h"

namespace rgb_matrix {
    template <typename T> RP2040_UART<T>::RP2040_UART(CFG *cfg) : Framebuffer<T>(cfg) {
            build_table(cfg->get_gamma(), cfg_->use_CIE1931());
            
            shutdown_ = false;
            start_ = false;
            thread_ = new std::thread(&RP2040_UART<T>::worker_thread, this);
    }

    template <typename T> RP2040_UART<T>::~RP2040_UART() {
        shutdown_ = true;

        if (thread_ != nullptr)
            thread_->join();
    }
    
    template <typename T> void RP2040_UART<T>::show() {
        start_ = true;

        while (start_);
    }

    template <typename T> void RP2040_UART<T>::worker_thread(RP2040_UART<T> *object) {
        uint32_t size = sizeof(T) * object->cfg_->get_cols() * object->cfg_->get_rows();
        char *start = (char *) "s";
        char *idle = (char *) "i";

        while (object->shutdown_) {
            if (object->start_) {
                // Send active tocken to get buss out of idle
                object->cfg_->get_node()->write(start, 1);

                // Wait for bus to become active (timout after 100uS and drop frame)
                for (int i = 0; i < 10; i++) {
                    // If it times out then the bus is idle, send frame
                    if (object->cfg_->get_node()->read(&idle, 1, 10) == 0) {
                        object->cfg_->get_node()->write((char *) object->buffer_, size);

                        // Check for idle loop, recover if still active
                        while (object->cfg_->get_node()->read(&idle, 1, 100) == 0)
                            object->cfg_->get_node()->write(idle, 1);
                    
                        break;
                    }
                }

                object->start_ = false;
            }
        }
    }
}