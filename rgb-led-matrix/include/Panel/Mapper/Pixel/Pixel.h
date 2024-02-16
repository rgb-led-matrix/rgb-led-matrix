#ifndef PIXEL_H
#define PIXEL_H

#include <Panel/Mapper/Mapper.h>
#include <CFG/types.h>

namespace rgb_matrix {
    class Pixel : public Mapper {
        public:
            Pixel(Mapper *panel);
            virtual ~Pixel();

            void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
            void SetPixel(cord_t cord, pixel_t pixel);

        protected:
            Pixel();

            virtual cord_t map_location(int x, int y) = 0;
            virtual Color_Order map_color(int x, int y) = 0;

            Mapper *panel_;

        private:
            void map();

            cord_t **locations_;
            Color_Order **orders_;
    };
}
#endif
