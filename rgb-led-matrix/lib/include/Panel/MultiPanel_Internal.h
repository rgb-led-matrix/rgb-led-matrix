#ifndef MULTIPANEL_INTERNAL_H
#define MULTIPANEL_INTERNAL_H

#include <mutex>
#include <list>
#include <Panel/MultiPanel.h>
#include <IO/Scheduler/Scheduler.h>
#include <ThreadPool/ThreadPool.h>

namespace rgb_matrix {
    class MultiPanel_Internal : public MultiPanel {
        public:
            MultiPanel_Internal(uint16_t width, uint16_t height);
            ~MultiPanel_Internal();

            void SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
            cord_t get_size();
            void set_brightness(uint8_t brightness);
            void map_wavelength(uint8_t color, Color index, uint16_t value);

            bool map_panel(uint16_t x, uint16_t y, Direction direction, Panel *panel, Protocol *protocol);
            void show(Control *control);

        protected:
            MultiPanel_Internal();

            struct Panel_t {
                Panel *panel;
                uint16_t x;
                uint16_t y;
                Direction direction;
                Protocol *protocol;
            };

            struct show_packet {
                MultiPanel_Internal *object;
                Panel_t *panel;
            };

            struct map_wavelength_packet {
                Panel_t *panel;
                uint8_t color;
                Color index;
                uint16_t value;
            };

            struct set_brightness_packet {
                Panel_t *panel;
                uint8_t brightness;
            };

            static void show_worker(volatile bool *result, show_packet args);
            static void map_wavelength_worker(volatile bool *result, map_wavelength_packet args);
            static void set_brightness_worker(volatile bool *result, set_brightness_packet args);

            static ThreadPool<volatile bool *, show_packet> *get_show_thread_pool();
            static ThreadPool<volatile bool *, map_wavelength_packet> *get_map_wavelength_thread_pool();
            static ThreadPool<volatile bool *, set_brightness_packet> *get_set_brightness_thread_pool();

            static ThreadPool<volatile bool *, show_packet> *show_thread_pool_;
            static ThreadPool<volatile bool *, set_brightness_packet> *set_brightness_thread_pool_;
            static ThreadPool<volatile bool *, map_wavelength_packet> *map_wavelength_thread_pool_;

            uint16_t width_;
            uint16_t height_;
            std::mutex lock_;
            std::list<Panel_t *> *panel_;
            pixel_t **pixel_;
            Scheduler *scheduler_;
    };
}
#endif
