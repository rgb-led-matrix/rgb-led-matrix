#ifndef TEST_NODE_H
#define TEST_NODE_H

#include <string>
#include <mutex>
#include "IO/Node/Node.h"

namespace rgb_matrix::Test_Node {
    class Test_Node : public Node {
        public:
            Test_Node(const char *file);

            // For Protocol
            void write(uint8_t *buf, uint32_t len);
            void read(uint8_t *buf, uint32_t *len, uint32_t timeout_us);

        protected:
            Test_Node();

            bool claim();
            void free();
            
            bool claim_;
            std::mutex lock_;
    };
}
#endif