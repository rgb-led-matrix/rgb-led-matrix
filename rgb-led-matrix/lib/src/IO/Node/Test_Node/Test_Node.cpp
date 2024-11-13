#include "IO/Node/Test_Node/Test_Node.h"
#include "Logger/Logger.h"

namespace rgb_matrix::Test_Node {
    Test_Node::Test_Node() {
        // Do not use this! 
    }

    Test_Node::Test_Node(const char *file) {
        // TODO:
    }

    void Test_Node::write(uint8_t *buf, uint32_t len) {
        // TODO:
    }

    void Test_Node::read(uint8_t *buf, uint32_t *len, uint32_t timeout_us) {
        // TODO:
    }

    bool Test_Node::claim() {
        bool result = false;

        lock_.lock();

        if (!claim_) {
            result = true;
            claim_ = true;
        }

        lock_.unlock();
        return result;
    }

    void Test_Node::free() {
        lock_.lock();
        claim_ = false;
        lock_.unlock();
    }
}
