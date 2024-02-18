#ifndef DOTCORRECT_H
#define DOTCORRECT_H

#include <stdint.h>
#include <mutex>

namespace rgb_matrix {
    struct DOTCorrect {
        public:
            DOTCorrect(uint16_t rows, uint16_t cols);
            ~DOTCorrect();

            bool set(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, float red, float green, float blue);
            void get(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, float *red, float *green, float *blue);

        protected:
            DOTCorrect();
        
        private:
            bool check(float f);

            uint16_t rows;
            uint16_t cols;
            float *table_;
            std::mutex lock_;
    };
}
#endif