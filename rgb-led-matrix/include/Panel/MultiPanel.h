#ifndef MULTIPANEL_H
#define MULTIPANEL_H

#include <Panel/Panel.h>

namespace rgb_matrix {
    // Pure interface for creating large panel from multiple panels
    //  Can be used to create weird setups
    class MultiPanel : public Drawer {
        public:
            enum Direction {
                Right = 0,
                Down = 1,
                Left = 2,
                Up = 3
            };

            virtual bool map_panel(uint16_t x, uint16_t y, Direction direction, Panel *panel, Protocol *protocol) = 0;
            virtual void show(Control *control) = 0;
    };
}
#endif
