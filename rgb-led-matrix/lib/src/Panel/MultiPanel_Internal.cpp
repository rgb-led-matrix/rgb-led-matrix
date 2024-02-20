#include <thread>
#include <algorithm>
#include <Panel/MultiPanel_Internal.h>
#include <Exception/Illegal.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    ThreadPool<volatile bool *, MultiPanel_Internal::show_packet> *MultiPanel_Internal::show_thread_pool_ = nullptr;
    ThreadPool<volatile bool *, MultiPanel_Internal::set_brightness_packet> *MultiPanel_Internal::set_brightness_thread_pool_ = nullptr;
    ThreadPool<volatile bool *, MultiPanel_Internal::map_wavelength_packet> *MultiPanel_Internal::map_wavelength_thread_pool_ = nullptr;

    // Do not use this!
    MultiPanel_Internal::MultiPanel_Internal() {
        throw Illegal("MultiPanel_Internal Panel");
    }

    MultiPanel_Internal::MultiPanel_Internal(uint16_t width, uint16_t height) : width_(width), height_(height) {
        scheduler_ = new Scheduler();
        pixel_ = new pixel_t *[width_];
        for (uint16_t i = 0; i < width_; i++)
            pixel_[i] = new pixel_t[height_];
    }

    MultiPanel_Internal::~MultiPanel_Internal() {
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            delete *it;
        panel_->clear();
        for (uint16_t i = 0; i < width_; i++)
            delete pixel_[i];
        delete pixel_;
        delete scheduler_;
    }

    bool MultiPanel_Internal::map_panel(uint16_t x, uint16_t y, Direction direction, Simple_Panel *panel, Protocol *protocol) {
        Panel_t *ptr = new Panel_t;

        if (panel == nullptr)
            throw Null_Pointer("Panel");

        if (protocol == nullptr)
            throw Null_Pointer("Protocol");

        if (x >= width_ || y >= height_)
            throw Illegal("Location");

        ptr->x = x;
        ptr->y = y;
        ptr->direction = direction;
        ptr->panel = panel;
        ptr->protocol = protocol;

        lock_.lock();
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it) {
            if (((*it)->panel == ptr->panel) || ((*it)->protocol == ptr->protocol)) {
                delete ptr;
                lock_.unlock();
                return false;
            }
        }

        scheduler_->add_protocol(protocol);
        panel_->push_back(ptr);
        lock_.unlock();
        return true;
    }

    void MultiPanel_Internal::SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue) {
        cord_t cord;
        pixel_t pixel;

        if (x >= width_ || y >= height_)
            throw Illegal("Location");

        lock_.lock();
        cord.x = x;
        cord.y = y;
        pixel.red = red;
        pixel.green = green;
        pixel.blue = blue;
        pixel_[cord.x][cord.y] = pixel;
        lock_.unlock();
    }

    cord_t MultiPanel_Internal::get_size() {
        cord_t result;
        lock_.lock();
        result.x = width_;
        result.y = height_;
        lock_.unlock();
        return result;
    }

    void MultiPanel_Internal::show() {
        lock_.lock();
        std::queue<volatile bool *> results;
        
        // Fork/Join Acceleration
        ThreadPool<volatile bool *, show_packet> *pool = get_show_thread_pool();
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it) {
            show_packet p;
            volatile bool *r;
            p.object = this;
            p.panel = (*it);
            r = new bool;
            *r = false;
            results.push(r);
            pool->submit(&MultiPanel_Internal::show_worker, r, p);
        }

        while (!results.empty()) {
            volatile bool *r = results.front();
            while(*r == false) {
                // Well hopefully the OS does something
                // std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
            results.pop();
        }

        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            (*it)->panel->show((*it)->protocol, false);

        scheduler_->start();
        lock_.unlock();
    }

    void MultiPanel_Internal::set_brightness(uint8_t brightness) {
        lock_.lock();
        std::queue<volatile bool *> results;

        // Fork/Join Acceleration
        ThreadPool<volatile bool *, set_brightness_packet> *pool = get_set_brightness_thread_pool();
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it) {
            set_brightness_packet p;
            volatile bool *r;
            p.panel = (*it);
            p.brightness = brightness;
            r = new bool;
            *r = false;
            pool->submit(&MultiPanel_Internal::set_brightness_worker, r, p);
        }

        while (!results.empty()) {
            volatile bool *r = results.front();
            while(*r == false) {
                // Well hopefully the OS does something
                // std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
            results.pop();
        }

        lock_.unlock();
    }

    void MultiPanel_Internal::map_wavelength(uint8_t color, Color index, uint16_t value) {
        lock_.lock();
        std::queue<volatile bool *> results;

        // Fork/Join Acceleration
        ThreadPool<volatile bool *, map_wavelength_packet> *pool = get_map_wavelength_thread_pool();
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it) {
            map_wavelength_packet p;
            volatile bool *r;
            p.panel = (*it);
            p.color = color;
            p.index = index;
            p.value = value;
            r = new bool;
            *r = false;
            pool->submit(&MultiPanel_Internal::map_wavelength_worker, r, p);
        }

        while (!results.empty()) {
            volatile bool *r = results.front();
            while(*r == false) {
                // Well hopefully the OS does something
                // std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
            results.pop();
        }

        lock_.unlock();
    }

    ThreadPool<volatile bool *, MultiPanel_Internal::show_packet> *MultiPanel_Internal::get_show_thread_pool() {
        if (show_thread_pool_ == nullptr) {
            uint8_t count = std::max(std::thread::hardware_concurrency() / 2, (unsigned int) 1);
            show_thread_pool_ = new ThreadPool<volatile bool *, MultiPanel_Internal::show_packet>();
            show_thread_pool_->start(count);
        }

        return show_thread_pool_;
    }

    ThreadPool<volatile bool *, MultiPanel_Internal::set_brightness_packet> *MultiPanel_Internal::get_set_brightness_thread_pool() {
        if (set_brightness_thread_pool_ == nullptr) {
            uint8_t count = std::max(std::thread::hardware_concurrency() / 2, (unsigned int) 1);
            set_brightness_thread_pool_ = new ThreadPool<volatile bool *, MultiPanel_Internal::set_brightness_packet>();
            set_brightness_thread_pool_->start(count);
        }

        return set_brightness_thread_pool_;
    }

    ThreadPool<volatile bool *, MultiPanel_Internal::map_wavelength_packet> *MultiPanel_Internal::get_map_wavelength_thread_pool() {
        if (map_wavelength_thread_pool_ == nullptr) {
            uint8_t count = std::max(std::thread::hardware_concurrency() / 2, (unsigned int) 1);
            map_wavelength_thread_pool_ = new ThreadPool<volatile bool *, MultiPanel_Internal::map_wavelength_packet>();
            map_wavelength_thread_pool_->start(count);
        }

        return map_wavelength_thread_pool_;
    }

    uint16_t range_helper(uint16_t map, uint16_t panel) {
        if (panel > map)
            return 0;
        else
            return map - panel;
    }

    // Must be read only!
    void MultiPanel_Internal::show_worker(volatile bool *result, show_packet args) {
        // Quick and dirty loop(s)!
        for (uint16_t x = 0; x < args.object->width_; x++) {
            for (uint16_t y = 0; y < args.object->height_; y++) {
                uint16_t map_x, map_y, panel_x, panel_y, range_x, range_y;
                pixel_t pixel;

                switch (args.panel->direction) {
                    case MultiPanel::Direction::Right:
                        map_x = args.panel->x;
                        map_y = args.panel->y;
                        panel_x = args.panel->panel->get_size().x;
                        panel_y = args.panel->panel->get_size().y;
                        range_x = map_x + panel_x;
                        range_y = map_y + panel_y;
                        pixel = args.object->pixel_[x][y];

                        if (((x >= map_x) &&  (x < range_x)) && ((y >= map_y) &&  (y < range_y))) {
                            args.panel->panel->SetPixel(x % panel_x, y % panel_y, pixel.red, pixel.green, pixel.blue);
                        }
                        break;
                    case MultiPanel::Direction::Down:
                        map_x = args.panel->x;
                        map_y = args.panel->y;
                        panel_x = args.panel->panel->get_size().x;
                        panel_y = args.panel->panel->get_size().y;
                        range_x = map_x + panel_y;
                        range_y = range_helper(map_y, panel_x);
                        pixel = args.object->pixel_[x][y];

                        if (((x >= map_x) &&  (x < range_x)) && ((y >= range_y) &&  (y < map_y))) {
                            args.panel->panel->SetPixel(x % panel_x, y % panel_y, pixel.red, pixel.green, pixel.blue);
                        }
                        break;
                    case MultiPanel::Direction::Left:
                        map_x = args.panel->x;
                        map_y = args.panel->y;
                        panel_x = args.panel->panel->get_size().x;
                        panel_y = args.panel->panel->get_size().y;
                        range_x = range_helper(map_x, panel_x);
                        range_y = range_helper(map_y, panel_y);
                        pixel = args.object->pixel_[x][y];

                        if (((x >= range_x) &&  (x < map_x)) && ((y >= range_y) &&  (y < map_y))) {
                            args.panel->panel->SetPixel(x % panel_x, y % panel_y, pixel.red, pixel.green, pixel.blue);
                        }
                        break;
                    case MultiPanel::Direction::Up:
                        map_x = args.panel->x;
                        map_y = args.panel->y;
                        panel_x = args.panel->panel->get_size().x;
                        panel_y = args.panel->panel->get_size().y;
                        range_x = range_helper(map_x, panel_y);
                        range_y = map_y + panel_x;
                        pixel = args.object->pixel_[x][y];

                        if (((x >= range_y) &&  (x < map_x)) && ((y >= map_y) &&  (y < range_y))) {
                            args.panel->panel->SetPixel(x % panel_x, y % panel_y, pixel.red, pixel.green, pixel.blue);
                        }
                        break;
                    default:
                        throw Unknown_Type("Direction");
                        break;
                }
            }
        }

        *result = true;
    }

    // Must be read only!
    void MultiPanel_Internal::map_wavelength_worker(volatile bool *result, map_wavelength_packet args) {
        args.panel->panel->map_wavelength(args.color, args.index, args.value);
        *result = true;
    }

    // Must be read only!
    void MultiPanel_Internal::set_brightness_worker(volatile bool *result, set_brightness_packet args) {
        args.panel->panel->set_brightness(args.brightness);
        *result = true;
    }
}
