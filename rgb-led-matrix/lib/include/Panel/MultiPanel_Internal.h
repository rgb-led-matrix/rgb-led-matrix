#ifndef MULTIPANEL_INTERNAL_H
#define MULTIPANEL_INTERNAL_H

#include <mutex>
#include <list>
#include "Panel/MultiPanel.h"
#include "IO/Scheduler/Scheduler.h"
#include "ThreadPool/ThreadPool.h"

namespace rgb_matrix {
    class MultiPanel_Internal : public MultiPanel {
        public:
            MultiPanel_Internal(uint16_t width, uint16_t height);
            ~MultiPanel_Internal();

            void SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
            cord_t get_size();
            void set_brightness(uint8_t brightness);
            void map_wavelength(uint8_t color, Color index, uint16_t value);

            bool map_panel(uint16_t x, uint16_t y, Direction direction, Panel *panel, Data_Protocol *protocol);
            void show(Control_Protocol *control);

        protected:
            MultiPanel_Internal();

            struct Panel_t {
                Panel *panel;
                uint16_t x;
                uint16_t y;
                Direction direction;
                Data_Protocol *protocol;
            };

            class show_packet : public Runnable {
                public:
                    void run();

                    MultiPanel_Internal *object;
                    Panel_t *panel;
                    bool result;
            };

            class map_wavelength_packet : public Runnable  {
                public:
                    void run();

                    Panel_t *panel;
                    uint8_t color;
                    Color index;
                    uint16_t value;
                    bool result;
            };

            class set_brightness_packet : public Runnable {
                public:
                    void run();

                    Panel_t *panel;
                    uint8_t brightness;
                    bool result;
            };

            static void show_worker(show_packet *args);
            static void map_wavelength_worker(map_wavelength_packet *args);
            static void set_brightness_worker(set_brightness_packet *args);

            uint16_t width_;
            uint16_t height_;
            std::mutex lock_;
            std::list<Panel_t *> *panel_;
            pixel_t **pixel_;
            Scheduler *scheduler_;
    };
}
#endif
