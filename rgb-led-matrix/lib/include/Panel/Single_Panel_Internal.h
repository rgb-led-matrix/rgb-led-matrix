#ifndef SINGLE_PANEL_INTERNAL_H
#define SINGLE_PANEL_INTERNAL_H

#include <mutex>
#include <CFG/CFG.h>
#include <Panel/Single_Panel.h>

namespace rgb_matrix {
    template <typename T> class Single_Panel_Internal : public Single_Panel {
        public:
            Single_Panel_Internal(CFG *cfg);
            ~Single_Panel_Internal();

            void SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
            void show(Protocol *protocol, bool schedule = true);
            cord_t get_size();
            
            void set_brightness(uint8_t brightness);
            void map_wavelength(uint8_t color, Color index, uint16_t value);

        protected:
            Single_Panel_Internal();
            void build_table();
            void MapColors(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, T *color);

            // Used by Pixel to resize into physical size.
            //  Config will have logic size. (They will contain the same number of pixels.)
            void resize(cord_t size);

            CFG *cfg_;
            T **buffer_;
            T lut[100][256];
            std::mutex lock_;
            volatile uint8_t brightness_;
            uint16_t width_;
            uint16_t height_;

            friend class Pixel;
    };
}
#endif
