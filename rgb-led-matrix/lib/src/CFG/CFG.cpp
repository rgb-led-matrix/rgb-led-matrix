#include <CFG/CFG.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    CFG::CFG() : dot_(8, 8) {
        throw Illegal("CFG: Attempted to use forbidden constructor.");
    }

    CFG::CFG(uint16_t rows, uint16_t cols, Data_Format_ID data_format, uint8_t scan, GAMMA gamma, Mapper *mapper) : dot_(rows, cols) {
        rows_ = rows;
        cols_ = cols;
        data_format_ = data_format;
        gamma_ = gamma;
        scan_ = scan;
        mapper_ = mapper;
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

    uint8_t CFG::get_scan() {
        return scan_;
    }

    Data_Format_ID CFG::get_data_format() { 
        return data_format_; 
    }

    Mapper *CFG::get_mapper() {
        return mapper_;
    }
};