#include <Logger/Logger.h>
#include <Exception/Null_Pointer.h>

namespace rgb_matrix {
    Logger *Logger::logger_ = nullptr;

    Logger *Logger::get_logger() {
        if (logger_ == nullptr)
            throw Null_Pointer("No logger");
        return logger_;
    }
}