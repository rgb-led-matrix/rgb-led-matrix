#ifndef CFG_H
#define CFG_H

#include <CFG/Types/types.h>
#include <CFG/Types/GAMMA.h>
#include <CFG/Types/DOTCorrect.h>

namespace rgb_matrix {
    class CFG {
        public:
            CFG(uint16_t rows, uint16_t cols, Data_Format_ID data_format, GAMMA gamma);

            DOTCorrect& get_dot();
            GAMMA& get_gamma();
            uint16_t get_rows();
            uint16_t get_cols();
            Data_Format_ID get_data_format();
        
        protected:
            CFG();

            uint16_t rows_;
            uint16_t cols_;
            DOTCorrect dot_;
            GAMMA gamma_;
            Data_Format_ID data_format_;
    };
}

#endif