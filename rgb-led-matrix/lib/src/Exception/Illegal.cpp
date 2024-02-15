#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    Illegal::Illegal() : String_Exception("Illegal") {
        // Do nothing
    }

    Illegal::Illegal(const char *str) : String_Exception(str) {
        // Do nothing
    }
}