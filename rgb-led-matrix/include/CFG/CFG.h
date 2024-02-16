#ifndef CFG_H
#define CFG_H

#include <vector>
#include <IO/Protocol/Protocol.h>
#include <CFG/types.h>
using namespace rgb_matrix;

namespace rgb_matrix {
    class CFG {
        public:
            CFG(int rows, int cols, Protocol *protocol, Data_Format_ID data_format, Panel_Type type, uint8_t scan, GAMMA gamma) : dot_(rows, cols) {
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
                protocol_ = protocol;
                data_format_ = data_format;
                gamma_ = gamma;
            }

            bool operator==(CFG& cfg) {
                return this->protocol_ == cfg.protocol_;
            }

            inline DOTCorrect& get_dot() { return dot_; }
            inline GAMMA& get_gamma() { return gamma_; }
            inline Protocol *get_protocol() { return protocol_; }
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
            Protocol *protocol_;
    };
}

#endif