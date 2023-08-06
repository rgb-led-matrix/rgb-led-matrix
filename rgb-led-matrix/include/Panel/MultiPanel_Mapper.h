#ifndef MULTIPANEL_MAPPER_H
#define MULTIPANEL_MAPPER_H

#include <list>
#include <thread>
#include "Panel/Panel_Pixel_Mapper.h"
using std::list;
using std::thread;

namespace rgb_matrix {
  class MultiPanel_Mapper : public Panel {
    public:
        MultiPanel_Mapper(int width, int height, int threads);
        virtual ~MultiPanel_Mapper();

        virtual void map_panel(int x, int y, Panel_Pixel_Mapper *panel);
        virtual void show();

        virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
        virtual void SetPixel(cord_t cord, pixel_t pixel);
        virtual cord_t get_size();

        // Applies to SetPixel calls after set
        virtual void set_brightness(uint8_t brightness);

    protected:
        MultiPanel_Mapper();

        struct Panel_t {
            Panel_Pixel_Mapper *panel;
            int x;
            int y;
        };

        list<Panel_t *> *panel_;
        list_t<thread *> *threads_;
  };
}
#endif
