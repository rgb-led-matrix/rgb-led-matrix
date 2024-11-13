#include "CFG/HUB75.h"

namespace rgb_matrix {
    HUB75::HUB75() {
        // Do not use this! 
    }

    HUB75::HUB75(uint16_t rows, uint16_t cols, Data_Format_ID data_format, uint8_t scan, GAMMA gamma, Mapper *mapper) : CFG(rows, cols, data_format, scan, gamma, mapper) {
        scan_ = scan * 2;
    }
};