#ifndef ROOT_H
#define ROOT_H

#include <IO/Node/Node.h>

namespace rgb_matrix {
    // Optional construct for Node factory (should hardware support fully independent notions)
    //  Note receiver card channels are dependent and should be considered a single Node
    class Root {
        public:
            virtual Node *GetNode(uint32_t index) = 0;
    };
}
#endif
