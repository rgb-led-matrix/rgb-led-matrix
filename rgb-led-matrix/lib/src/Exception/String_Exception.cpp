#include <Exception/String_Exception.h>
#include <Logger/Logger.h>

namespace rgb_matrix {
    String_Exception::String_Exception() : std::exception() {
        prefix_ = "";
        string_ = "String Exception: Attempted to use forbidden constructor.";
    }

    String_Exception::String_Exception(const char *str, bool log) : std::exception() {
        string_ = prefix_ + std::string(str);
        
        if (log)
            Logger::get_logger()->write(Logger::Level::ERROR, string_.c_str());
    }

    const char *String_Exception::what() const throw() {
        return string_.c_str();
    }
}