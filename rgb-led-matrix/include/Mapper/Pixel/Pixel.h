#ifndef PIXEL_H
#define PIXEL_H

#include <Panel/Single_Panel.h>
#include <CFG/types.h>

namespace rgb_matrix {
    // Optional construct for handling pixel locations and RGB ordering
    class Pixel {
        public:
            void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);

        protected:
            virtual cord_t map_location(int x, int y) = 0;
            virtual Color_Order map_color(int x, int y) = 0;

            Single_Panel *panel_;

        private:
            virtual void map() = 0;

            cord_t **locations_;
            Color_Order **orders_;
    };
}
#endif
