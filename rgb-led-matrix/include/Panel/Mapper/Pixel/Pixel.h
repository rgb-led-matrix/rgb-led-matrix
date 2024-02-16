#ifndef PIXEL_H
#define PIXEL_H

#include <Panel/Panel.h>
#include <CFG/types.h>

namespace rgb_matrix {
    class Pixel : public Mapper {
        public:
            Pixel(Panel *panel);
            virtual ~Pixel();

            void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);

        protected:
            Pixel();

            virtual cord_t map_location(int x, int y) = 0;
            virtual Color_Order map_color(int x, int y) = 0;

            Panel *panel_;

        private:
            void map();

            cord_t **locations_;
            Color_Order **orders_;
    };
}
#endif
