#ifndef LOGGER_H
#define LOGGER_H

namespace rgb_matrix {
    class Logger {
        public:
            enum Level {
                DEBUG,
                INFO,
                WARN,
                ERROR
            };

            virtual void write(Level level, const char *str);

            static Logger *get_logger();

        protected:
            static Logger *logger_;
    };
}
#endif
