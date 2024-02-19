#ifndef PANEL_H
#define PANEL_H

#include <Panel/Simple_Panel.h>

namespace rgb_matrix {
    // Pure interface for Single Panel
    class Panel : public Simple_Panel {
        protected:
            // Used by Multiplex to resize into physical size.
            //  Config will have logic size. (They will contain the same number of pixels.)
            virtual void resize(cord_t size) = 0;

            friend class Multiplex;
    };
}
#endif
