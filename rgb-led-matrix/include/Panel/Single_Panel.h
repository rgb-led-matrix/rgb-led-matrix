#ifndef SINGLE_PANEL_H
#define SINGLE_PANEL_H

#include <Panel/Panel.h>

namespace rgb_matrix {
    // Pure interface for Single Panel
    class Single_Panel : public Panel {
        public:
            virtual void show(Protocol *protocol) = 0;
    };
}
#endif
