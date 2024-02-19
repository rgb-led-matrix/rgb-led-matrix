#ifndef SIMPLE_PANEL_H
#define SIMPLE_PANEL_H

#include <Mapper/Mapper.h>
#include <IO/Protocol/Protocol.h>

namespace rgb_matrix {
    // Pure interface for Single Panel
    class Simple_Panel : public Mapper {
        public:
            // Using Single_Panel directly should always schedule (we may delete this option)
            virtual void show(Protocol *protocol, bool schedule = true) = 0;
    };
}
#endif
