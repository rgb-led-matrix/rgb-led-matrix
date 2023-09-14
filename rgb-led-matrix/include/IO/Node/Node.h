#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include <IO/Protocol/Protocol.h>

namespace rgb_matrix {
    class Node {
        public:
            Node() : protocol_(nullptr) {}
            virtual ~Node() {}

            virtual void send(uint8_t *buf, uint32_t size) = 0;
            virtual bool process(uint8_t stages) = 0;
            virtual void set_protocol(Protocol *protocol) { protocol_ = protocol; }

        protected:
            Protocol *protocol_;
    };
}
#endif
