#ifndef HUB75_H
#define HUB75_H

#include <CFG/CFG.h>

namespace rgb_matrix {
    class HUB75 : public CFG {
        public:
            HUB75(uint16_t rows, uint16_t cols, Data_Format_ID data_format, uint8_t scan, GAMMA gamma, Mapper *mapper = nullptr);

        protected:
            HUB75();
    };
}

#endif