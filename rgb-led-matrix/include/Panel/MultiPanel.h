#ifndef MULTIPANEL_H
#define MULTIPANEL_H

#include <Panel/Single_Panel.h>

namespace rgb_matrix {
    // Pure interface for creating large panel from multiple panels
    //  Can be used to create weird setups
    class MultiPanel : public Mapper {
        public:
            enum Direction {
                Right = 0,
                Down = 1,
                Left = 2,
                Up = 3
            };

            virtual bool map_panel(uint16_t x, uint16_t y, Direction direction, Single_Panel *panel, Protocol *protocol) = 0;
            virtual void show() = 0;
    };
}
#endif
