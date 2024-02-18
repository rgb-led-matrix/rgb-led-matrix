#ifndef OUTDOOR_H
#define OUTDOOR_H

#include <Mapper/Pixel/Pixel.h>

namespace rgb_matrix {
    class Outdoor : public Pixel {
        public:
            Outdoor(Single_Panel *panel);

            cord_t get_size();

        protected:
            Outdoor();

            cord_t get_actual_size();
            cord_t map_location(uint16_t x, uint16_t y);
            Color_Order map_color(uint16_t x, uint16_t y);
    };
}
#endif
