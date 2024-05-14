#ifndef PANEL_H
#define PANEL_H

#include <Drawer/Drawer.h>
#include <IO/Protocol/Protocol.h>

namespace rgb_matrix {
    // Pure interface for Panel
    class Panel : public Drawer {
        public:
            // Using Panel directly should always schedule (we may delete this option)
            virtual void show(Data_Protocol *protocol, Control_Protocol *control, bool schedule = true) = 0;
    };
}
#endif
