#include <thread>
#include <algorithm>
#include <Panel/MultiPanel_Internal.h>
#include <Exception/Illegal.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    ThreadPool<void *, MultiPanel_Internal::show_packet> *MultiPanel_Internal::show_thread_pool_ = nullptr;
    ThreadPool<void *, MultiPanel_Internal::set_brightness_packet> *MultiPanel_Internal::set_brightness_thread_pool_ = nullptr;
    ThreadPool<void *, MultiPanel_Internal::map_wavelength_packet> *MultiPanel_Internal::map_wavelength_thread_pool_ = nullptr;

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

        ThreadPool<void *, show_packet> *pool = get_show_thread_pool();
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it) {
            show_packet p;
            p.object = this;
            p.panel = (*it);
            pool->submit(&MultiPanel_Internal::show_worker, nullptr, p);
        }

        // TODO: Wait for them to complete (look at return values???)

        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            (*it)->panel->show((*it)->protocol, false);

        scheduler_->start();
        lock_.unlock();
    }


    void MultiPanel_Internal::set_brightness(uint8_t brightness) {
        lock_.lock();
        /*uint32_t size = panel_->size();
        std::list<std::thread *> threads;
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end();) {
            uint32_t num = std::min(size, num_threads);

            for (uint32_t i = 0; i < num; i++) {
                threads.push_back(new std::thread(&MultiPanel_Internal::set_brightness_worker, (*it), brightness));
                ++it;
            }

            for (uint32_t i = 0; i < num; i++) {
                std::thread *t = threads.front();
                threads.pop_front();
                t->join();
                delete t;
            }

            size -= num;

            if (size == 0)
                break;
        }*/
        lock_.unlock();
    }

    void MultiPanel_Internal::map_wavelength(uint8_t color, Color index, uint16_t value) {
        lock_.lock();
        /*uint32_t size = panel_->size();
        std::list<std::thread *> threads;
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end();) {
            uint32_t num = std::min(size, num_threads);

            for (uint32_t i = 0; i < num; i++) {
                threads.push_back(new std::thread(&MultiPanel_Internal::map_wavelength_worker, (*it), color, index, value));
                 ++it;
            }

            for (uint32_t i = 0; i < num; i++) {
                std::thread *t = threads.front();
                threads.pop_front();
                t->join();
                delete t;
            }

            size -= num;

            if (size == 0)
                break;
        }*/

        lock_.unlock();
    }

    ThreadPool<void *, MultiPanel_Internal::show_packet> *MultiPanel_Internal::get_show_thread_pool() {
        if (show_thread_pool_ == nullptr) {
            // TODO:
        }

        return show_thread_pool_;
    }

    ThreadPool<void *, MultiPanel_Internal::set_brightness_packet> *MultiPanel_Internal::get_set_brightness_thread_pool() {
        if (set_brightness_thread_pool_ == nullptr) {
            // TODO:
        }

        return set_brightness_thread_pool_;
    }

    ThreadPool<void *, MultiPanel_Internal::map_wavelength_packet> *MultiPanel_Internal::get_map_wavelength_thread_pool() {
        if (map_wavelength_thread_pool_ == nullptr) {
            // TODO:
        }

        return map_wavelength_thread_pool_;
    }

    // Must be read only!
    void MultiPanel_Internal::show_worker(void *, show_packet args) {
        // Quick and dirty loop(s)!
        for (uint16_t x = 0; x < args.object->width_; x++) {
            for (uint16_t y = 0; y < args.object->height_; y++) {
                switch (args.panel->direction) {
                    case MultiPanel::Direction::Right:
                        if (((x >= args.panel->x) &&  (x < (args.panel->x + args.panel->panel->get_size().x))) && ((y >= args.panel->y) &&  (y < (args.panel->y + args.panel->panel->get_size().y)))) {
                            args.panel->panel->SetPixel(x % args.panel->panel->get_size().x, y % args.panel->panel->get_size().y, args.object->pixel_[x][y].red, args.object->pixel_[x][y].green, args.object->pixel_[x][y].blue);
                        }
                        break;
                    case MultiPanel::Direction::Down:       // TODO: Finish
                    case MultiPanel::Direction::Left:       // TODO: Finish
                    case MultiPanel::Direction::Up:         // TODO: Finish
                        String_Exception("Not implemented");
                        break;
                    default:
                        throw Unknown_Type("Direction");
                        break;
                }
            }
        }
    }

    // Must be read only!
    void MultiPanel_Internal::map_wavelength_worker(void *, map_wavelength_packet args) {
        args.panel->panel->map_wavelength(args.color, args.index, args.value);
    }

    // Must be read only!
    void MultiPanel_Internal::set_brightness_worker(void *, set_brightness_packet args) {
        args.panel->panel->set_brightness(args.brightness);
    }
}
