#ifndef PANEL_INTERNAL_H
#define PANEL_INTERNAL_H

#include <mutex>
#include <CFG/CFG.h>
#include <Panel/Panel.h>

namespace rgb_matrix {
    template <typename T> class Panel_Internal : public Panel {
        public:
            Panel_Internal(CFG *cfg);
            ~Panel_Internal();

            void SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
            cord_t get_size();
            void set_brightness(uint8_t brightness);
            void map_wavelength(uint8_t color, Color index, uint16_t value);
            
            void show(Protocol *protocol, bool schedule = true);

        protected:
            Panel_Internal();

            void build_table();
            void MapColors(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, T *color);

            CFG *cfg_;
            T **buffer_;
            T lut[100][256];
            std::mutex lock_;
            cord_t **locations_;
            Color_Order **orders_;
            volatile uint8_t brightness_;
            uint16_t width_;
            uint16_t height_;
    };
}
#endif
