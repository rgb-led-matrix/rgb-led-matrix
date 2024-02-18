#ifndef SINGLE_PANEL_INTERNAL_H
#define SINGLE_PANEL_INTERNAL_H

#include <mutex>
#include <stdint.h>
#include <CFG/CFG.h>
#include <Panel/Single_Panel.h>
using std::mutex;

namespace rgb_matrix {
    template <typename T> class Single_Panel_Internal : public Single_Panel {
        public:
            Single_Panel_Internal(CFG *cfg);
            ~Single_Panel_Internal();

            void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
            void show(Protocol *protocol, bool schedule = true, bool threadless = false);
            cord_t get_size();
            
            void set_brightness(uint8_t brightness);
            void map_wavelength(uint8_t color, Color index, uint16_t value);

        protected:
            Single_Panel_Internal();
            void build_table();
            void MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, T *color);

            CFG *cfg_;
            T **buffer_;
            T lut[100][256];
            mutex lock_;
            volatile uint8_t brightness_;
    };
}
#endif
