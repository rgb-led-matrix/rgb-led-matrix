#ifndef CFG_H
#define CFG_H

#include <CFG/Types/types.h>
#include <CFG/Types/GAMMA.h>
#include <CFG/Types/DOTCorrect.h>
#include <Mapper/Mapper.h>

namespace rgb_matrix {
    class CFG {
        public:
            CFG(uint16_t rows, uint16_t cols, Data_Format_ID data_format, uint8_t scan, GAMMA gamma, Mapper *mapper = nullptr);

            DOTCorrect& get_dot();
            GAMMA& get_gamma();
            uint16_t get_rows();
            uint16_t get_cols();
            uint8_t get_scan();
            Data_Format_ID get_data_format();
            Mapper *get_mapper();
        
        protected:
            CFG();

            uint16_t rows_;
            uint16_t cols_;
            uint8_t scan_;
            DOTCorrect dot_;
            GAMMA gamma_;
            Data_Format_ID data_format_;
            Mapper *mapper_;
    };
}

#endif