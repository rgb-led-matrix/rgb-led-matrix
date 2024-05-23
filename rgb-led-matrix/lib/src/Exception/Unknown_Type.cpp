#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    // Do not use this!
    Unknown_Type::Unknown_Type() : String_Exception("Unknown Type: Attempted to use forbidden constructor.") {
        prefix_ = "Unknown Type: ";
    }

    Unknown_Type::Unknown_Type(const char *str, bool log) : String_Exception(str, log) {
        // Do nothing
    }
}