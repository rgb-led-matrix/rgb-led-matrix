#include <CFG/CFG.h>
#include <Exception/Unknown_Type.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    CFG::CFG() : dot_(8, 8) {
        throw Illegal("CFG");
    }

    CFG::CFG(int rows, int cols, Data_Format_ID data_format, Panel_Type type, uint8_t scan, GAMMA gamma) : dot_(rows, cols) {
        switch (type) {
            case Panel_Type::HUB75:
                rows_ = scan * 2;
                cols_ = rows / (scan * 2) * cols;
                break;
            case Panel_Type::STANDARD:
                rows_ = rows;
                cols_ = cols;
                break;
            default:
                throw Unknown_Type("Panel Types");
                break;
        }

        dot_ = DOTCorrect(rows_, cols_);
        data_format_ = data_format;
        gamma_ = gamma;
    }

    DOTCorrect& CFG::get_dot() { 
        return dot_;
    }
    
    GAMMA& CFG::get_gamma() { 
        return gamma_; 
    }

    int CFG::get_rows() { 
        return rows_; 
    }

    int CFG::get_cols() { 
        return cols_; 
    }

    Data_Format_ID CFG::get_data_format() { 
        return data_format_; 
    }
};