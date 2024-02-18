#include <CFG/CFG.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    CFG::CFG() : dot_(8, 8) {
        throw Illegal("CFG");
    }

    CFG::CFG(uint16_t rows, uint16_t cols, Data_Format_ID data_format, GAMMA gamma) : dot_(rows, cols) {
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

    uint16_t CFG::get_rows() { 
        return rows_; 
    }

    uint16_t CFG::get_cols() { 
        return cols_; 
    }

    Data_Format_ID CFG::get_data_format() { 
        return data_format_; 
    }
};