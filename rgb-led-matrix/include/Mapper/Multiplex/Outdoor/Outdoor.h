#ifndef OUTDOOR_H
#define OUTDOOR_H

#include <Mapper/Multiplex/Multiplex.h>

namespace rgb_matrix {
    class Outdoor : public Multiplex {
        public:
            Outdoor(Panel *panel);

        protected:
            Outdoor();

            cord_t get_actual_size();
            cord_t map_location(uint16_t x, uint16_t y);
    };
}
#endif
