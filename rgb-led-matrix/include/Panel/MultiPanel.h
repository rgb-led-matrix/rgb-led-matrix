#ifndef MULTIPANEL_H
#define MULTIPANEL_H

#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <list>
#include <Panel/Panel.h>
using std::vector;
using std::thread;
using std::mutex;
using std::queue;
using std::list;

namespace rgb_matrix {
    class MultiPanel : public Panel {
        public:
                MultiPanel(int width, int height);
                ~MultiPanel();

                bool map_panel(int x, int y, CFG *cfg);
                void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
                cord_t get_size();
                void show();

                // Use these before calling SetPixel! (Has no effect until after.)
                //  We are stuck with these here, because we are hiding Framebuffer/RGB. (Templating)
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
    };
}
#endif
