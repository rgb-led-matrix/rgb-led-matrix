#include <Panel/MultiPanel_Internal.h>
#include <Exception/Illegal.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
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

    bool MultiPanel_Internal::map_panel(uint16_t x, uint16_t y, Direction direction, Panel *panel, Data_Protocol *protocol) {
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

    void MultiPanel_Internal::show(Control_Protocol *control) {
        lock_.lock();
        std::queue<show_packet *> results;
        
        // Fork/Join Acceleration
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it) {
            show_packet *p = new show_packet();
            p->object = this;
            p->panel = (*it);
            p->result = false;
            results.push(p);
            ThreadPool::get_threadpool(ThreadPool::Pool_ID::Drawer)->submit(p);
        }

        while (!results.empty()) {
            bool *r = &results.front()->result;
            while(*r == false) {
                // Well hopefully the OS does something
                // std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
            delete results.front();
            results.pop();
        }

        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            (*it)->panel->show((*it)->protocol, nullptr, false);

        scheduler_->start(control);
        lock_.unlock();
    }

    void MultiPanel_Internal::set_brightness(uint8_t brightness) {
        lock_.lock();
        std::queue<set_brightness_packet *> results;

        // Fork/Join Acceleration
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it) {
            set_brightness_packet *p = new set_brightness_packet();
            p->panel = (*it);
            p->brightness = brightness;
            p->result = false;
            results.push(p);
            ThreadPool::get_threadpool(ThreadPool::Pool_ID::Drawer)->submit(p);
        }

        while (!results.empty()) {
            bool *r = &results.front()->result;
            while(*r == false) {
                // Well hopefully the OS does something
                // std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
            delete results.front();
            results.pop();
        }

        lock_.unlock();
    }

    void MultiPanel_Internal::map_wavelength(uint8_t color, Color index, uint16_t value) {
        lock_.lock();
        std::queue<map_wavelength_packet *> results;

        // Fork/Join Acceleration
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it) {
            map_wavelength_packet *p = new map_wavelength_packet();
            p->panel = (*it);
            p->color = color;
            p->index = index;
            p->value = value;
            p->result = false;
            results.push(p);
            ThreadPool::get_threadpool(ThreadPool::Pool_ID::Drawer)->submit(p);
        }

        while (!results.empty()) {
            bool *r = &results.front()->result;
            while(*r == false) {
                // Well hopefully the OS does something
                // std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
            delete results.front();
            results.pop();
        }

        lock_.unlock();
    }

    uint16_t range_helper(uint16_t map, uint16_t panel) {
        if (panel > map)
            return 0;
        else
            return map - panel;
    }

    // Must be read only!
    void MultiPanel_Internal::show_worker(show_packet *args) {
        // Quick and dirty loop(s)!
        for (uint16_t x = 0; x < args->object->width_; x++) {
            for (uint16_t y = 0; y < args->object->height_; y++) {
                uint16_t map_x, map_y, panel_x, panel_y, range_x, range_y;
                pixel_t pixel;

                switch (args->panel->direction) {
                    case MultiPanel::Direction::Right:
                        map_x = args->panel->x;
                        map_y = args->panel->y;
                        panel_x = args->panel->panel->get_size().x;
                        panel_y = args->panel->panel->get_size().y;
                        range_x = map_x + panel_x;
                        range_y = map_y + panel_y;
                        pixel = args->object->pixel_[x][y];

                        if (((x >= map_x) &&  (x < range_x)) && ((y >= map_y) &&  (y < range_y))) {
                            args->panel->panel->SetPixel(x % panel_x, y % panel_y, pixel.red, pixel.green, pixel.blue);
                        }
                        break;
                    case MultiPanel::Direction::Down:
                        map_x = args->panel->x;
                        map_y = args->panel->y;
                        panel_x = args->panel->panel->get_size().x;
                        panel_y = args->panel->panel->get_size().y;
                        range_x = map_x + panel_y;
                        range_y = range_helper(map_y, panel_x);
                        pixel = args->object->pixel_[x][y];

                        if (((x >= map_x) &&  (x < range_x)) && ((y >= range_y) &&  (y < map_y))) {
                            args->panel->panel->SetPixel(x % panel_x, y % panel_y, pixel.red, pixel.green, pixel.blue);
                        }
                        break;
                    case MultiPanel::Direction::Left:
                        map_x = args->panel->x;
                        map_y = args->panel->y;
                        panel_x = args->panel->panel->get_size().x;
                        panel_y = args->panel->panel->get_size().y;
                        range_x = range_helper(map_x, panel_x);
                        range_y = range_helper(map_y, panel_y);
                        pixel = args->object->pixel_[x][y];

                        if (((x >= range_x) &&  (x < map_x)) && ((y >= range_y) &&  (y < map_y))) {
                            args->panel->panel->SetPixel(x % panel_x, y % panel_y, pixel.red, pixel.green, pixel.blue);
                        }
                        break;
                    case MultiPanel::Direction::Up:
                        map_x = args->panel->x;
                        map_y = args->panel->y;
                        panel_x = args->panel->panel->get_size().x;
                        panel_y = args->panel->panel->get_size().y;
                        range_x = range_helper(map_x, panel_y);
                        range_y = map_y + panel_x;
                        pixel = args->object->pixel_[x][y];

                        if (((x >= range_y) &&  (x < map_x)) && ((y >= map_y) &&  (y < range_y))) {
                            args->panel->panel->SetPixel(x % panel_x, y % panel_y, pixel.red, pixel.green, pixel.blue);
                        }
                        break;
                    default:
                        throw Unknown_Type("Direction");
                        break;
                }
            }
        }

        args->result = true;
    }

    // Must be read only!
    void MultiPanel_Internal::map_wavelength_worker(map_wavelength_packet *args) {
        args->panel->panel->map_wavelength(args->color, args->index, args->value);
        args->result = true;
    }

    // Must be read only!
    void MultiPanel_Internal::set_brightness_worker(set_brightness_packet *args) {
        args->panel->panel->set_brightness(args->brightness);
        args->result = true;
    }

    void MultiPanel_Internal::show_packet::run() {
        MultiPanel_Internal::show_worker(this);
    }

    void MultiPanel_Internal::map_wavelength_packet::run() {
        MultiPanel_Internal::map_wavelength_worker(this);
    }

    void MultiPanel_Internal::set_brightness_packet::run() {
        MultiPanel_Internal::set_brightness_worker(this);
    }
}
