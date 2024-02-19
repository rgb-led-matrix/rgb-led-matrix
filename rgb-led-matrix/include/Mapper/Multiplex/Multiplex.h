#ifndef MULTIPLEX_H
#define MULTIPLEX_H

#include <Panel/Panel.h>

namespace rgb_matrix {
    // Optional construct for handling Multiplex locations
    class Multiplex : public Simple_Panel {
        public:
            Multiplex(Panel *panel);
            virtual ~Multiplex();

            void SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
            void set_brightness(uint8_t brightness);
            void map_wavelength(uint8_t color, Color index, uint16_t value);
            void show(Protocol *protocol, bool schedule = true);
            cord_t get_size();

        protected:
            Multiplex();

            // This will be the physical size rather than the logic size reported by get_size from Mapper
            virtual cord_t get_actual_size() = 0;
            virtual cord_t map_location(uint16_t x, uint16_t y) = 0;
            virtual Color_Order map_color(uint16_t x, uint16_t y);

            void map();

            Panel *full_panel_;
            Simple_Panel *panel_;
            cord_t **locations_;
            Color_Order **orders_;
            cord_t size_;
    };
}
#endif
