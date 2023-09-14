#ifndef RGBMATRIX_H
#define RGBMATRIX_H

#include <stddef.h>
#include <Panel/Panel.h>
#include <CFG/types.h>

namespace rgb_matrix {
    class RGBMatrix {
        public:
            static Panel *CreatePanel(CFG *cfg);
    };
}
#endif
