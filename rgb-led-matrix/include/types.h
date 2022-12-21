#ifndef TYPES_H
#define TYPES_H

namespace rgb_matrix {
    class CFG;

    enum Canvas_ID {
        RP2040_Multiplexed_PMP_ID = 0,
        HUB75_BCM_ID = 1
    };

    struct GAMMA {
        GAMMA() : red(2.2), green(2.2), blue (2.2) {}

        float red;
        float green;
        float blue;
    };

    struct DOTCorrect {
        public:
        DOTCorrect(int rows, int cols);
        ~DOTCorrect();

        bool set(int x, int y, float red, float green, float blue);
        void get(int x, int y, float *red, float *green, float *blue);

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