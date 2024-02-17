#ifndef RGBMATRIX_H
#define RGBMATRIX_H

#include <Panel/Single_Panel.h>
#include <Panel/MultiPanel.h>
#include <CFG/CFG.h>

namespace rgb_matrix {
    // Factory for creating individual panels from configuration
    class RGBMatrix {
        public:
            static Single_Panel *Create_Single_Panel(CFG *cfg);
            static MultiPanel *Create_MultiPanel(int width, int height);
    };
}
#endif
