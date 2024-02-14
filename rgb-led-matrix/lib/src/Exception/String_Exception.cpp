#include <Exception/String_Exception.h>

namespace rgb_matrix {
    String_Exception::String_Exception() : std::exception() {
        // Do nothing
    }

    String_Exception::String_Exception(const char *str) : std::exception() {
        string_ = std::string(str);
    }

    const char *String_Exception::what() const throw() {
        return string_.c_str();
    }
}