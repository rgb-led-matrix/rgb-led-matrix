#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    // Do not use this!
    Unknown_Type::Unknown_Type() : String_Exception("Unknown_Type") {
        // Do nothing
    }

    Unknown_Type::Unknown_Type(const char *str) : String_Exception(str) {
        // Do nothing
    }
}