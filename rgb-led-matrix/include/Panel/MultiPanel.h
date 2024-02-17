#ifndef MULTIPANEL_H
#define MULTIPANEL_H

#include <Panel/Single_Panel.h>
#include <IO/Protocol/Protocol.h>

namespace rgb_matrix {
    // Pure interface for creating large panel from multiple panels
    //  Can be used to create weird setups
    class MultiPanel : public Mapper {
        public:
            virtual bool map_panel(int x, int y, Single_Panel *panel, Protocol *protocol) = 0;
            virtual void show() = 0;
            virtual void set_brightness(uint8_t brightness) = 0;
            virtual void map_wavelength(uint8_t color, Color index, uint16_t value) = 0;
    };
}
#endif
