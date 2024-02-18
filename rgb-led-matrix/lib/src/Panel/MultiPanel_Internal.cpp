#include <Panel/MultiPanel_Internal.h>
#include <Exception/Illegal.h>
#include <Exception/Null_Pointer.h>

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

    bool MultiPanel_Internal::map_panel(uint16_t x, uint16_t y, Single_Panel *panel, Protocol *protocol) {
        Panel_t *ptr = new Panel_t;

        if (panel == nullptr)
            throw Null_Pointer("Panel");

        if (protocol == nullptr)
            throw Null_Pointer("Protocol");

        if (x >= width_ || y >= height_)
            throw Illegal("Location");

        ptr->x = x;
        ptr->y = y;
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

        // Quick and dirty loop(s)!
        for (uint16_t x = 0; x < width_; x++) {
            for (uint16_t y = 0; y < height_; y++) {
                for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it) {
                    if (((x >= (*it)->x) &&  (x < ((*it)->x + (*it)->panel->get_size().x))) && ((y >= (*it)->y) &&  (y < ((*it)->y + (*it)->panel->get_size().y)))) {
                        (*it)->panel->SetPixel(x % (*it)->panel->get_size().x, y % (*it)->panel->get_size().y, pixel_[x][y].red, pixel_[x][y].green, pixel_[x][y].blue);
                    }
                }
            }
        }

        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            (*it)->panel->show((*it)->protocol, false);

        scheduler_->start();
        lock_.unlock();
    }


    void MultiPanel_Internal::set_brightness(uint8_t brightness) {
        lock_.lock();

        // Quick and dirty loop
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            (*it)->panel->set_brightness(brightness);

        lock_.unlock();
    }

    void MultiPanel_Internal::map_wavelength(uint8_t color, Color index, uint16_t value) {
        lock_.lock();

        // Quick and dirty loop
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            (*it)->panel->map_wavelength(color, index, value);

        lock_.unlock();
    }
}
