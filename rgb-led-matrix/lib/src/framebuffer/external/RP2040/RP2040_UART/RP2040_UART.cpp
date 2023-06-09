#include <assert.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "framebuffer/external/external.h"
#include "framebuffer/external/RP2040/RP2040_UART/RP2040_UART.h"

namespace rgb_matrix {
    template <typename T> RP2040_UART<T>::RP2040_UART(CFG *cfg) 
        : Framebuffer<T>(cfg) {
            thread_ = nullptr; 

            if (cfg->get_id() == Canvas_ID::RP2040_UART_ID)
                cfg_ = static_cast<RP2040_UART_CFG *>(cfg);
            else
                throw cfg;

            if (cfg_->use_gamma_correction())
                build_table(cfg->get_gamma(), cfg_->use_CIE1931());
            else
                build_table(GAMMA(1.0, 1.0, 1.0), cfg_->use_CIE1931());
            
            shutdown_ = false;
            start_ = false;
            thread_ = new std::thread(&RP2040_UART<T>::worker_thread, this);
    }

    template <typename T> RP2040_UART<T>::~RP2040_UART() {
        shutdown_ = true;

        if (thread_ != nullptr)
            thread_->join();
    }
    
    template <typename T> void RP2040_UART<T>::DumpToMatrix() {
        start_ = true;

        while (start_);
    }

    // Handles dot correction and PWM bit scaling
    template <typename T> inline void  RP2040_UART<T>::MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue) {
        float fr, fg, fb;
        uint8_t bright = cfg_->use_brightness() ? cfg_->get_brightness() % 100 : 100;

        if (cfg_->use_dot_correction()) {
            cfg_->get_dot().get(x, y, r, g, b, &fr, &fg, &fb);
            *red = (uint16_t) round(lut[r][bright][0] / 65535.0 * fr * cfg_->get_pwm_bits());
            *green = (uint16_t) round(lut[g][bright][1] / 65535.0 * fg * cfg_->get_pwm_bits());
            *blue = (uint16_t) round(lut[b][bright][2] / 65535.0 * fb * cfg_->get_pwm_bits());
        }
        else {
            *red = lut[r][bright][0] * cfg_->get_pwm_bits() / 65535;
            *green = lut[g][bright][1] * cfg_->get_pwm_bits() / 65535;
            *blue = lut[b][bright][2] * cfg_->get_pwm_bits() / 65535;
        }
    }

    // Handles brightness, gamma and CIE1931
    template <typename T> void RP2040_UART<T>::build_table(GAMMA g, bool use_CIE1931) {
        if (!use_CIE1931) {
            for (uint32_t i = 0; i < 256; i++) {
                for (int j = 0; j < 100; j++) {
                    constexpr uint32_t lim = 65535;
                    lut[i][j][0] = (uint16_t) round(pow(i / 255.0, 1 / g.get_red()) * lim * j / 99.0);
                    lut[i][j][1] = (uint16_t) round(pow(i / 255.0, 1 / g.get_green()) * lim * j / 99.0);
                    lut[i][j][2] = (uint16_t) round(pow(i / 255.0, 1 / g.get_blue()) * lim * j / 99.0);
                }
            }
        }
        else {
            for (uint32_t i = 0; i < 256; i++) {
                for (int j = 0; j < 100; j++) {
                    constexpr uint32_t lim = 65535;
                    float temp = pow(i / 255.0, 1 / g.get_red()) * j;
                    lut[i][j][0] = (uint16_t) round(lim * ((temp <= 8) ? temp / 902.3 : pow((temp + 16) / 116.0, 3)));
                    temp = pow(i / 255.0, 1 / g.get_green()) * j;
                    lut[i][j][1] = (uint16_t) round(lim * ((temp <= 8) ? temp / 902.3 : pow((temp + 16) / 116.0, 3)));
                    temp = pow(i / 255.0, 1 / g.get_blue()) * j;
                    lut[i][j][2] = (uint16_t) round(lim * ((temp <= 8) ? temp / 902.3 : pow((temp + 16) / 116.0, 3)));
                }
            }
        }
    }

    template <typename T> void RP2040_UART<T>::worker_thread(RP2040_UART<T> *object) {
        uint32_t size = sizeof(T) * object->shared_mapper_->width() * object->shared_mapper_->height();
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
                        object->cfg_->get_node()->write((char *) object->shared_mapper_->buffer(), size);

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

    template class RP2040_UART<PixelDesignator>;
}