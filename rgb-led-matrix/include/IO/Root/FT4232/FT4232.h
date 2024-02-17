#ifndef FT4232_H
#define FT4232_H

#include <string>
#include <IO/Root/Root.h>

namespace rgb_matrix {
    class FT4232 : public Root {
        public:
            FT4232(const char *serial_number);

            Node *GetNode(uint32_t index);
        
        protected:
            FT4232();

            std::string serial_number_;
    };
}
#endif
