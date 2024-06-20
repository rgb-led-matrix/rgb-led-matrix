#include <Exception/Timeout.h>

namespace rgb_matrix {
    // Do not use this!
    Timeout::Timeout() : String_Exception() {
        prefix_ = "Timeout: ";
    }

    Timeout::Timeout(const char *str, bool log) : String_Exception(str, log) {
        // Do nothing
    }
}