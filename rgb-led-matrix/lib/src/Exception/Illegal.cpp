#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    Illegal::Illegal() : String_Exception("Illegal: Attempted to use forbidden constructor.") {
        prefix_ = "Illegal: ";
    }

    Illegal::Illegal(const char *str, bool log) : String_Exception(str, log) {
        // Do nothing
    }
}