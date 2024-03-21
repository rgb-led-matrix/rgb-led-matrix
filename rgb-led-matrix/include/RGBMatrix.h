#ifndef RGBMATRIX_H
#define RGBMATRIX_H

#include <Panel/Panel.h>
#include <CFG/CFG.h>

namespace rgb_matrix {
    // Factory for creating individual panels from configuration
    class RGBMatrix {
        public:
            static Panel *Create_Panel(CFG *cfg);
    };
}
#endif
