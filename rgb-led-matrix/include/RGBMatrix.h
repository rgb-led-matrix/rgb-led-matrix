#ifndef RGBMATRIX_H
#define RGBMATRIX_H

#include <stddef.h>
#include <Panel/Single_Panel.h>
#include <CFG/CFG.h>

namespace rgb_matrix {
    // Factory for creating individual panels from configuration
    class RGBMatrix {
        public:
            static Single_Panel *CreatePanel(CFG *cfg);
    };
}
#endif
