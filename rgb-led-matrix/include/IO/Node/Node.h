#ifndef NODE_H
#define NODE_H

#include <stdint.h>

namespace rgb_matrix {
    // Required construct for OSI Layer 1 and below
    //  Should support streams and/or memory maps. (OS or device driver may be required.)
    class Node {
        public:
            // For Protocol
            //  TODO: Refactor to buffered streams using packets
            //      There is an issue with CPU usage due to this
            virtual void write(uint8_t *buf, uint32_t len) = 0;
            void write(uint32_t val);
            void write(uint16_t val);
            void write(uint8_t val);
            virtual void read(uint8_t *buf, uint32_t *len, uint32_t timeout_us) = 0;

        protected:
            // Should be thread-safe
            virtual bool claim() = 0;
            virtual void free() = 0;

            friend class Data_Protocol;
            friend class Control_Protocol;

        private:
            template <typename T> void write_internal(T val);
    };
}
#endif
