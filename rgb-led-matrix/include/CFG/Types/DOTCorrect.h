#ifndef DOTCORRECT_H
#define DOTCORRECT_H

#include <stdint.h>

namespace rgb_matrix {
    struct DOTCorrect {
        public:
            DOTCorrect(int rows, int cols);
            ~DOTCorrect();

            bool set(int x, int y, uint8_t r, uint8_t g, uint8_t b, float red, float green, float blue);
            void get(int x, int y, uint8_t r, uint8_t g, uint8_t b, float *red, float *green, float *blue);

        protected:
            DOTCorrect();
        
        private:
            bool check(float f);

            int rows;
            int cols;
            float *table_;
    };
}
#endif