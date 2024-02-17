#ifndef MULTIPANEL_INTERNAL_H
#define MULTIPANEL_INTERNAL_H

#include <mutex>
#include <list>
#include <Panel/MultiPanel.h>
#include <IO/Scheduler/Scheduler.h>
using std::mutex;
using std::list;

namespace rgb_matrix {
    class MultiPanel_Internal : public MultiPanel {
        public:
            MultiPanel_Internal(int width, int height);
            ~MultiPanel_Internal();

            bool map_panel(int x, int y, Single_Panel *panel, Protocol *protocol);

            void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
            cord_t get_size();
            void show();
            void set_brightness(uint8_t brightness);
            void map_wavelength(uint8_t color, Color index, uint16_t value);

        protected:
            MultiPanel_Internal();

            struct Panel_t {
                Single_Panel *panel;
                int x;
                int y;
                Protocol *protocol;
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
