#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

namespace rgb_matrix {
    class CFG;

    enum Panel_Type {
        STANDARD = 0,
        HUB75 = 1,
    };

    struct cord_t {
        int x;
        int y;
    };

    enum Color_Order {
        RGB = 0,
        RBG = 1,
        BRG = 2,
        BGR = 3,
        GRB = 4,
        GBR = 5,
    };

    enum Color {
        Red = 0,
        Green = 1,
        Blue = 2,
    };

    struct pixel_t {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    enum External_ID {
        RP2040_UART_ID = 0,
    };

    enum Data_Format_ID {
        RGB48_ID = 0,
        RGB24_ID = 1,
        RGB_222_ID = 2,
        RGB_555_ID = 3,
    };

    struct GAMMA {
        public:
            GAMMA(float red = 1.0, float green = 1.0, float blue = 1.0) : red_(red), green_(green), blue_(blue) {}

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
        
        private:
            bool check(float f);

            int rows;
            int cols;
            float *table_;
    };
}

#endif