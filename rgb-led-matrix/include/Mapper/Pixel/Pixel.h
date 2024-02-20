#ifndef PIXEL_H
#define PIXEL_H

#include <Mapper/Multiplex/Multiplex.h>

namespace rgb_matrix {
    // Optional construct for handling Pixel locations and RGB ordering
    class Pixel : public Multiplex {
        public:
            Pixel(Multiplex *panel);

        protected:
            Pixel();

            // This will be the physical size rather than the logic size reported by get_size from Mapper
            cord_t get_actual_size();

            // Implement map_location and map_color
    };
}
#endif
