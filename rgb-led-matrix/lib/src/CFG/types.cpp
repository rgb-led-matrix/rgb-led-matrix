#include "CFG/CFG.h"

namespace rgb_matrix {

    Options::Options(const CFG *config) : cfg(config), multiplexing(0), pixel_mapper_config(NULL) {}

    DOTCorrect::DOTCorrect(int r, int c) : rows(r), cols(c) {
        table_ = new float[256 * 3 * rows * cols];

        for (int i = 0; i < 256; i++) {
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
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

    bool DOTCorrect::set(int x, int y, uint8_t r, uint8_t g, uint8_t b, float red, float green, float blue) {
        if (check(red) || check(green) || check(blue))
        return false;

        table_[(3 * rows * cols * r) + (3 * ((y * cols) + x))] = red;
        table_[(3 * rows * cols * g) + (3 * ((y * cols) + x)) + 1] = green;
        table_[(3 * rows * cols * b) + (3 * ((y * cols) + x)) + 2] = blue;

        return true;
    }

    void DOTCorrect::get(int x, int y, uint8_t r, uint8_t g, uint8_t b, float *red, float *green, float *blue) {
        *red = table_[(3 * rows * cols * r) + (3 * ((y * cols) + x))];
        *green = table_[(3 * rows * cols * g) + (3 * ((y * cols) + x)) + 1];
        *blue = table_[(3 * rows * cols * b) + (3 * ((y * cols) + x)) + 2];
    }

    bool DOTCorrect::check(float f) {
        return !(f < 0.5 || f > 1.0);
    }
};