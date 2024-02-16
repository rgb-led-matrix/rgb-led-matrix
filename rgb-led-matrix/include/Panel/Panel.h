#ifndef PANEL_H
#define PANEL_H

#include <stdint.h>
#include <CFG/types.h>
#include <Mapper/Mapper.h>
#include <Event/Event.h>

namespace rgb_matrix {
    class Panel : public Mapper, Event {
        public:
            virtual void set_brightness(uint8_t brightness) = 0;
            virtual void map_wavelength(uint8_t color, Color index, uint16_t value) = 0;
    };
}
#endif
