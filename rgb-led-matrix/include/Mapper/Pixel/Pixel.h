#ifndef PIXEL_H
#define PIXEL_H

#include <Panel/Multiplex/Multiplex.h>

namespace rgb_matrix {
    // Optional construct for handling Pixel locations and RGB ordering
    class Pixel : public Multiplex {
        public:
            Pixel(Multiplex *panel);

        protected:
            Pixel();

            // This will be the physical size rather than the logic size reported by get_size from Mapper
            virtual cord_t get_actual_size();
            virtual cord_t map_location(uint16_t x, uint16_t y);
            virtual Color_Order map_color(uint16_t x, uint16_t y);
    };
}
#endif
