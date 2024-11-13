#ifndef RGBMATRIX_H
#define RGBMATRIX_H

#include "Panel/MultiPanel.h"
#include "CFG/CFG.h"

namespace rgb_matrix {
    // Factory for creating individual panels from configuration
    class RGBMatrix {
        public:
            static Panel *Create_Panel(CFG *cfg);
            static MultiPanel *Create_MultiPanel(uint16_t width, uint16_t height);
    };
}
#endif
