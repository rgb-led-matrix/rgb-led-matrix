#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

namespace rgb_matrix {
    class CFG;

    enum Canvas_ID {
        RP2040_ID = 0,
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

    struct Options {
        Options(CFG *config, int multiplexing, const char *pixel_mapper_config) : cfg_(config), multiplexing_(multiplexing), pixel_mapper_config_(pixel_mapper_config) {}

        inline CFG *get_cfg() { return cfg_; }
        inline int get_multiplexing() { return multiplexing_; }
        inline const char *get_pixel_mapper_config() { return pixel_mapper_config_; }

        protected:
            CFG *cfg_;
            int multiplexing_;
            const char *pixel_mapper_config_;
    };
}

#endif