#ifndef ILLEGAL_Hprotocols_
#define ILLEGAL_H

#include <Exception/String_Exception.h>

namespace rgb_matrix {
    class Illegal : public String_Exception {
        public:
            Illegal(const char *str, bool log = true);

        protected:
            Illegal();
    };
}
#endif
