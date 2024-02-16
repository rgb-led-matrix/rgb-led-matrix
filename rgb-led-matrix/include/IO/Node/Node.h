#ifndef NODE_H
#define NODE_H

#include <stdint.h>

namespace rgb_matrix {
    class Node {
        public:
            // For Protocol
            virtual void write(char *buf, uint32_t len) = 0;
            virtual int read(char **buf, uint32_t len, uint32_t timeout_us) = 0;
    };
}
#endif
