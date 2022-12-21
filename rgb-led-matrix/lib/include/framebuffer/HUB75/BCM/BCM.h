#ifndef BCM_H
#define BCM_H

#include "framebuffer/framebuffer.h"
#include "port/gpio/gpio.h"
#include "port/thread/Thread.h"

namespace rgb_matrix {
    template <typename T> class BCM : public Framebuffer<T> {
        public:
            BCM(CFG *cfg);
            ~BCM() {}

            virtual void DumpToMatrix();

        protected:
            virtual void  MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue);

            GPIO *io;
        
        private:
            void InitGPIO();
    };
}
#endif