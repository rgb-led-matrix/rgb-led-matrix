#include <chrono>
#include <thread>
#include <Panel/MultiPanel_Internal.h>
#include <Exception/Illegal.h>
#include <Exception/Null_Pointer.h>
#include <RGBMatrix.h>
#include <CFG/CFG.h>

namespace rgb_matrix {
    // Do not use this!
    MultiPanel_Internal::MultiPanel_Internal() {
        throw Illegal("MultiPanel_Internal Panel");
    }

    MultiPanel_Internal::MultiPanel_Internal(int width, int height) : width_(width), height_(height) {
        throw String_Exception("Not finished");

        scheduler_ = new Scheduler();
        pixel_ = new pixel_t *[width_];
        for (int i = 0; i < width_; i++)
            pixel_[i] = new pixel_t[height_];
    }

    MultiPanel_Internal::~MultiPanel_Internal() {
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            delete *it;
        panel_->clear();
        for (int i = 0; i < width_; i++)
            delete pixel_[i];
        delete pixel_;
        delete scheduler_;
    }

    bool MultiPanel_Internal::map_panel(int x, int y, Single_Panel *panel, Protocol *protocol) {
        Panel_t *ptr = new Panel_t;

        if (panel == nullptr)
            throw Null_Pointer("Panel");

        if (protocol == nullptr)
            throw Null_Pointer("Protocol");

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

    void MultiPanel_Internal::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
        cord_t cord;
        pixel_t pixel;

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
        result.x = width_;
        result.y = height_;
        return result;
    }

    void MultiPanel_Internal::show() {
        lock_.lock();
        // TODO: Convert pixel_ to panels_[x]->panel->set_pixel (implement runnables?)
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            (*it)->panel->show((*it)->protocol, true);
        scheduler_->start();
        while(!scheduler_->isFinished()) std::this_thread::sleep_for (std::chrono::seconds(1));
        lock_.unlock();
    }


    void MultiPanel_Internal::set_brightness(uint8_t brightness) {
        lock_.lock();
        // Implement runnables?
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            (*it)->panel->set_brightness(brightness);
        lock_.unlock();
    }

    void MultiPanel_Internal::map_wavelength(uint8_t color, Color index, uint16_t value) {
        lock_.lock();
        // Implement runnables?
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            (*it)->panel->map_wavelength(color, index, value);
        lock_.unlock();
    }
}
