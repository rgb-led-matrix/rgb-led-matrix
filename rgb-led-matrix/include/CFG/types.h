#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

namespace rgb_matrix {
    class CFG;

    enum Canvas_ID {
        RP2040_UART_ID = 0,
    };

    struct GAMMA {
        GAMMA(float red, float green, float blue) : red_(red), green_(green), blue_(blue) {}

        inline float get_red() { return red_; }
        inline float get_green() { return green_; }
        inline float get_blue() { return blue_; }

        protected:
            float red_;
            float green_;
            float blue_;
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
}

#endif