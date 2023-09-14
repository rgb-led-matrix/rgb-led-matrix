#ifndef RP2040_H
#define RP2040_H

#include <vector>
#include <thread>
#include "framebuffer/Framebuffer.h"
#include "CFG/external/RP2040/RP2040_CFG.h"

namespace rgb_matrix {
    template <typename T> class RP2040: public Framebuffer<T> {
        public:
            RP2040(CFG *cfg);

        protected:
            virtual void  MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, T *pixel);

        private:
            RP2040_CFG *cfg_;
    };
}

#endif