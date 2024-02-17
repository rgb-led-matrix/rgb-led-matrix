#ifndef PANEL_H
#define PANEL_H

#include <stdint.h>
#include <CFG/Types/types.h>
#include <Mapper/Mapper.h>
#include <IO/Protocol/Protocol.h>

namespace rgb_matrix {
    // Pure interface for Panel management
    class Panel : public Mapper {
        public:
            virtual void set_brightness(uint8_t brightness) = 0;
            virtual void map_wavelength(uint8_t color, Color index, uint16_t value) = 0;
    };
}
#endif
