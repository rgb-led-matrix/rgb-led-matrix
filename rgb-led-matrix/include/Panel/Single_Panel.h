#ifndef SINGLE_PANEL_H
#define SINGLE_PANEL_H

#include <Mapper/Mapper.h>
#include <IO/Protocol/Protocol.h>

namespace rgb_matrix {
    // Pure interface for Single Panel
    class Single_Panel : public Mapper {
        public:
            virtual void show(Protocol *protocol) = 0;
    };
}
#endif
