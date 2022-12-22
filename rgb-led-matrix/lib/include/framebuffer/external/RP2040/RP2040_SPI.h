#ifndef RP2040_H
#define RP2040_H

#include <vector>
#include "framebuffer/framebuffer.h"
#include "CFG/RP2040_SPI_CFG.h"

namespace rgb_matrix {
    template <typename T> class RP2040_SPI: public Framebuffer<T> {
        public:
            RP2040_SPI(CFG *cfg);
            ~RP2040_SPI() {}

            virtual void DumpToMatrix();

        protected:
            virtual void  MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue);

        private:
            struct table {
                table(GAMMA g, bool use_CIE1931);

                uint16_t val[256][100][3];
            };

            std::vector<std::vector<table>> lut;
            RP2040_SPI_CFG *cfg_;
    };
}

#endif