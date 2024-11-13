#ifndef TIMEOUT_H
#define TIMEOUT_H

#include "Exception/String_Exception.h"

namespace rgb_matrix {
    class Timeout : public String_Exception {
        public:
            Timeout(const char *str, bool log = true);

        protected:
            Timeout();
    };
}
#endif
