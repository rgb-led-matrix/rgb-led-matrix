#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

namespace rgb_matrix {
    class CFG;

    enum Canvas_ID {
        RP2040_SPI_ID = 0,
    };

    struct GAMMA {
        GAMMA(float red = 2.2, float green = 2.2, float blue = 2.2) : red(red), green(green), blue (blue) {}

        float red;
        float green;
        float blue;
    };

    struct DOTCorrect {
        public:
        DOTCorrect(int rows, int cols);
        ~DOTCorrect();

        bool set(int x, int y, uint8_t r, uint8_t g, uint8_t b, float red, float green, float blue);
        void get(int x, int y, uint8_t r, uint8_t g, uint8_t b, float *red, float *green, float *blue);

        // TODO: Limit access
        int rows;
        int cols;
        
        private:
        bool check(float f);

        float *table_;
    };

    struct Options {
        Options(CFG *cfg);

        CFG *cfg;

        int multiplexing;
        const char *pixel_mapper_config;
        const char *hardware_mapping;
    };
}

#endif