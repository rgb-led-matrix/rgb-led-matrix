#ifndef GAMMA_H
#define GAMMA_H

namespace rgb_matrix {
    struct GAMMA {
        public:
            GAMMA(float red = 1.0, float green = 1.0, float blue = 1.0);

            float get_red();
            float get_green();
            float get_blue();

        protected:
            float red_;
            float green_;
            float blue_;
    };
}
#endif