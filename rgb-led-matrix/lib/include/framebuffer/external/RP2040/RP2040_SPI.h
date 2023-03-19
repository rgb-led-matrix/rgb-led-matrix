#ifndef RP2040_H
#define RP2040_H

#include <vector>
#include "framebuffer/framebuffer.h"
#include "CFG/RP2040_SPI_CFG.h"

namespace rgb_matrix {
    template <typename T> class RP2040_SPI: public Framebuffer<T> {
        public:
            RP2040_SPI(CFG *cfg);
            ~RP2040_SPI();

            virtual void DumpToMatrix();

        protected:
            virtual void  MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue);

        private:
            void build_table(GAMMA g, bool use_CIE1931);
            void init_spi(const char *spi);
            void write(uint8_t *buf, uint32_t len);

            uint16_t lut[256][100][3];
            RP2040_SPI_CFG *cfg_;
            int fd;
    };
}

#endif