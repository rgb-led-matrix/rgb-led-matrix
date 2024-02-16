#ifndef UNKNOWN_TYPE_H
#define UNKNOWN_TYPE_H

#include <Exception/String_Exception.h>

namespace rgb_matrix {
    class Unknown_Type : public String_Exception {
        public:
            Unknown_Type(const char *str);

        protected:
            Unknown_Type();
    };
}
#endif
