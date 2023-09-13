#ifndef MULTIPANEL_MAPPER_H
#define MULTIPANEL_MAPPER_H

#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <list>
#include "Panel/Panel_Pixel_Mapper.h"
using std::vector;
using std::thread;
using std::mutex;
using std::queue;
using std::list;

namespace rgb_matrix {
  class MultiPanel_Mapper {
    public:
        MultiPanel_Mapper(int width, int height, int threads = 1);
        virtual ~MultiPanel_Mapper();

        virtual void map_panel(int x, int y, Panel_Pixel_Mapper *panel);
        virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
        virtual void SetPixel(cord_t cord, pixel_t pixel);
        virtual cord_t get_size();
        virtual void show();

        // Applies to show calls after set/map
        virtual void set_brightness(uint8_t brightness);
        virtual void map_wavelength(uint8_t color, Color index, uint16_t value);

    protected:
        MultiPanel_Mapper();

        static void thread(void *args);

        struct Panel_t {
            Panel_Pixel_Mapper *panel;
            int x;
            int y;
        };

        virtual void task(Panel_t *panel);

        int width_;
        int height_;
        int thread_count_;
        mutex lock_;
        list<Panel_t *> *panel_;
        vector<std::thread> *threads_;
        queue<Panel_t *> queue_;
        mutex queue_lock_;
        volatile bool shutdown_; 
        pixel_t **pixel_;
  };
}
#endif
