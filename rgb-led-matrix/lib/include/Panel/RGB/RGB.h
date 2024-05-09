#ifndef FRAMEBUFFER_RGB_H
#define FRAMEBUFFER_RGB_H

#include <stdint.h>
#include <CFG/Types/types.h>

namespace rgb_matrix {
    class RGB {
        public:
            static uint8_t translate_id(Data_Format_ID id);
    };
}

#endif