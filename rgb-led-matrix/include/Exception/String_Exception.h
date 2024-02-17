#ifndef STRING_EXCEPTION_H
#define STRING_EXCEPTION_H

#include <exception>
#include <string>

namespace rgb_matrix {
    class String_Exception : public std::exception {
        public:
            String_Exception(const char *str);

            virtual const char *what() const throw();

        protected:
            String_Exception();

            std::string string_;
    };
}
#endif
