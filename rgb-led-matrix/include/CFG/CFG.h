#ifndef CFG_H
#define CFG_H

#include <CFG/Types/types.h>
#include <CFG/Types/GAMMA.h>
#include <CFG/Types/DOTCorrect.h>

namespace rgb_matrix {
    class CFG {
        public:
            CFG(int rows, int cols, Data_Format_ID data_format, GAMMA gamma);

            DOTCorrect& get_dot();
            GAMMA& get_gamma();
            int get_rows();
            int get_cols();
            Data_Format_ID get_data_format();
        
        protected:
            CFG();

            int rows_;
            int cols_;
            DOTCorrect dot_;
            GAMMA gamma_;
            Data_Format_ID data_format_;
    };
}

#endif