#ifndef PANEL_H
#define PANEL_H

#include <Drawer/Drawer.h>
#include <IO/Protocol/Protocol.h>

namespace rgb_matrix {
    // Pure interface for Simple Panel
    class Panel : public Drawer {
        public:
            // Using Simple Panel directly should always schedule (we may delete this option)
            virtual void show(Protocol *protocol, bool schedule = true) = 0;

        protected:
    };
}
#endif
