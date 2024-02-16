#ifndef PANEL_H
#define PANEL_H

#include <stdint.h>
#include <CFG/types.h>
#include <Panel/Mapper/Mapper.h>
#include <Panel/Event.h>

namespace rgb_matrix {
    class Panel : public Mapper, Event {
        public:
            // Use these before calling SetPixel! (Has no effect until after.)
            //  We are stuck with these here, because we are hiding Framebuffer/RGB. (Templating)
            virtual void set_brightness(uint8_t brightness) = 0;
            virtual void map_wavelength(uint8_t color, Color index, uint16_t value) = 0;
    };
}
#endif
