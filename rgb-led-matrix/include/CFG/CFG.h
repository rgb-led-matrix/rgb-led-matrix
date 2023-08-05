#ifndef CFG_H
#define CFG_H

#include <vector>
#include <IO/Node/Node.h>
#include "types.h"
using namespace rgb_matrix;

namespace rgb_matrix {
  class CFG {
    public:
      CFG(int rows, int cols, Node *node, Panel_Type type, uint8_t scan) : dot_(rows, cols) {
        switch (type) {
          case Panel_Type::HUB75:
            rows_ = scan * 2;
            cols_ = rows / (scan * 2) * cols;
            break;
          default:
            rows_ = rows;
            cols_ = cols;
            break;
        }

        dot_ = DOTCorrect(rows_, cols_);
        node_ = node;
      }

      inline Panel_ID get_id() { return id_; }
      inline DOTCorrect& get_dot() { return dot_; }
      inline GAMMA& get_gamma() { return gamma_; }
      inline Node *get_node() { return node_; }
      inline int get_rows() { return rows_; }
      inline int get_cols() { return cols_; }
    
    protected:
      virtual bool isValid() = 0; 

      int rows_;
      int cols_;
      DOTCorrect dot_;
      GAMMA gamma_;
      Panel_ID id_;
      Node *node_;
  };
}

#endif