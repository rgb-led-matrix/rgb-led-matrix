#ifndef SINGLE_PANEL_H
#define SINGLE_PANEL_H

#include <Mapper/Mapper.h>
#include <IO/Protocol/Protocol.h>

namespace rgb_matrix {
    // Pure interface for Single Panel
    class Single_Panel : public Mapper {
        public:
            virtual void show(Protocol *protocol) = 0;
            virtual void set_brightness(uint8_t brightness) = 0;
            virtual void map_wavelength(uint8_t color, Color index, uint16_t value) = 0;
    };
}
#endif
