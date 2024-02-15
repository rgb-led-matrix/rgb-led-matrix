#ifndef PANEL_PIXEL_MAPPER_H
#define PANEL_PIXEL_MAPPER_H

#include <Panel/Panel.h>
#include <CFG/types.h>

namespace rgb_matrix {
    class Panel_Pixel_Mapper : public Panel {
        public:
            Panel_Pixel_Mapper(Panel *panel);
            virtual ~Panel_Pixel_Mapper();

            void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
            void SetPixel(cord_t cord, pixel_t pixel);
            void show();
            cord_t get_size();
            Protocol *get_protocol();

            // Applies to SetPixel calls after set/map
            void set_brightness(uint8_t brightness);
            void map_wavelength(uint8_t color, Color index, uint16_t value);

        protected:
            Panel_Pixel_Mapper();

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
