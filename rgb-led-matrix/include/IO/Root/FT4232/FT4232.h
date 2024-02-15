#ifndef FT4232_H
#define FT4232_H

#include <IO/Root/Root.h>

namespace rgb_matrix {
    class FT4232 : public Root {
        public:
            FT4232(const char *serial_number);
            ~FT4232();

            Node *GetNode(uint32_t index);
        
        protected:
            FT4232();
            Node *nodes_[4];
    };
}
#endif
