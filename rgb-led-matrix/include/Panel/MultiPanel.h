#ifndef MULTIPANEL_H
#define MULTIPANEL_H

#include <mutex>
#include <list>
#include <Panel/Panel.h>
#include <IO/Scheduler/Scheduler.h>
using std::mutex;
using std::list;

namespace rgb_matrix {
    // Factory for creating large panel from multiple panels
    //  Can be used to create weird setups
    class MultiPanel : public Panel {
        public:
            MultiPanel(int width, int height);
            ~MultiPanel();

            bool map_panel(int x, int y, CFG *cfg);

            void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
            cord_t get_size();
            void show();
            void set_brightness(uint8_t brightness);
            void map_wavelength(uint8_t color, Color index, uint16_t value);

        protected:
            MultiPanel();

            struct Panel_t {
                Panel *panel;
                int x;
                int y;
                CFG *cfg;
            };

            int width_;
            int height_;
            mutex lock_;
            list<Panel_t *> *panel_;
            pixel_t **pixel_;
            Scheduler *scheduler_;
    };
}
#endif
