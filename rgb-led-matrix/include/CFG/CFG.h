#ifndef CFG_H
#define CFG_H

#include <CFG/types.h>

namespace rgb_matrix {
    class CFG {
        public:
            CFG(int rows, int cols, Data_Format_ID data_format, Panel_Type type, uint8_t scan, GAMMA gamma) : dot_(rows, cols) {
                switch (type) {
                    case Panel_Type::HUB75:
                        rows_ = scan * 2;
                        cols_ = rows / (scan * 2) * cols;
                        break;
                    case Panel_Type::STANDARD:
                    default:
                        rows_ = rows;
                        cols_ = cols;
                        break;
                }

                dot_ = DOTCorrect(rows_, cols_);
                data_format_ = data_format;
                gamma_ = gamma;
            }

            inline DOTCorrect& get_dot() { return dot_; }
            inline GAMMA& get_gamma() { return gamma_; }
            inline int get_rows() { return rows_; }
            inline int get_cols() { return cols_; }
            inline Data_Format_ID get_data_format() { return data_format_; }
        
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