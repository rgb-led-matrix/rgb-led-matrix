#include <CFG/CFG.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    DOTCorrect::DOTCorrect() {
        throw Illegal("DOT Correct");
    }

    DOTCorrect::DOTCorrect(uint16_t r, uint16_t c) : rows(r), cols(c) {
        table_ = new float[256 * 3 * rows * cols];

        for (uint16_t i = 0; i < 256; i++) {
            for (uint16_t y = 0; y < rows; ++y) {
                for (uint16_t x = 0; x < cols; ++x) {
                table_[(3 * rows * cols * i) + (3 * ((y * cols) + x))] = 1.0;
                table_[(3 * rows * cols * i) + (3 * ((y * cols) + x)) + 1] = 1.0;
                table_[(3 * rows * cols * i) + (3 * ((y * cols) + x)) + 2] = 1.0;
                }
            }
        }
    }

    DOTCorrect::~DOTCorrect() {
        delete table_;
    }

    bool DOTCorrect::set(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, float red, float green, float blue) {
        if (check(red) || check(green) || check(blue))
            throw Illegal("Value");

        if (x >= cols || y >= rows)
            throw Illegal("Location");

        table_[(3 * rows * cols * r) + (3 * ((y * cols) + x))] = red;
        table_[(3 * rows * cols * g) + (3 * ((y * cols) + x)) + 1] = green;
        table_[(3 * rows * cols * b) + (3 * ((y * cols) + x)) + 2] = blue;

        return true;
    }

    void DOTCorrect::get(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, float *red, float *green, float *blue) {
        if (x >= cols || y >= rows)
            throw Illegal("Location");

        *red = table_[(3 * rows * cols * r) + (3 * ((y * cols) + x))];
        *green = table_[(3 * rows * cols * g) + (3 * ((y * cols) + x)) + 1];
        *blue = table_[(3 * rows * cols * b) + (3 * ((y * cols) + x)) + 2];
    }

    bool DOTCorrect::check(float f) {
        return !(f < 0.5 || f > 1.0);
    }
};