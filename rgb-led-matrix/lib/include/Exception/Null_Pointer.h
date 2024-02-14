#ifndef NULL_POINTER_H
#define NULL_POINTER_H

#include <Exception/String_Exception.h>
#include <string>

namespace rgb_matrix {
    class Null_Pointer : public String_Exception {
        public:
            Null_Pointer(const char *str);

        protected:
            Null_Pointer();
    };
}
#endif
