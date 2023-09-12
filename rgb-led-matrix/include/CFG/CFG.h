#ifndef CFG_H
#define CFG_H

#include <vector>
#include <IO/Node/Node.h>
#include "types.h"
using namespace rgb_matrix;

namespace rgb_matrix {
  class CFG {
    public:
      CFG(int rows, int cols, Node *node, Data_Format_ID data_format, Panel_Type type, uint8_t scan) : dot_(rows, cols) {
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
        node_ = node;
        data_format_ = data_format;
      }

      inline External_ID get_id() { return id_; }
      inline DOTCorrect& get_dot() { return dot_; }
      inline GAMMA& get_gamma() { return gamma_; }
      inline Node *get_node() { return node_; }
      inline int get_rows() { return rows_; }
      inline int get_cols() { return cols_; }
      inline Data_Format_ID get_data_format() { return data_format_; }
    
    protected:
      virtual bool isValid() = 0; 

      int rows_;
      int cols_;
      DOTCorrect dot_;
      GAMMA gamma_;
      External_ID id_;
      Data_Format_ID data_format_;
      Node *node_;
  };
}

#endif