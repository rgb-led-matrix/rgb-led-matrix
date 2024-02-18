#ifndef SINGLE_PANEL_H
#define SINGLE_PANEL_H

#include <Mapper/Mapper.h>
#include <IO/Protocol/Protocol.h>

namespace rgb_matrix {
    // Pure interface for Single Panel
    class Single_Panel : public Mapper {
        public:
            // Using Single_Panel directly should always schedule and use a thread (we may delete this option)
            virtual void show(Protocol *protocol, bool schedule = true, bool threadless = false) = 0;
    };
}
#endif
