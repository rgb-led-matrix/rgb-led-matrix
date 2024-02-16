#include <chrono>
#include <Panel/MultiPanel.h>
#include <Exception/Illegal.h>
#include <Exception/Null_Pointer.h>
#include <RGBMatrix.h>
#include <CFG/CFG.h>

namespace rgb_matrix {
    // Do not use this!
    MultiPanel::MultiPanel() {
        throw Illegal("MultiPanel Panel");
    }

    MultiPanel::MultiPanel(int width, int height) : width_(width), height_(height) {
        throw String_Exception("Not finished");

        // TODO:

        scheduler_ = new Scheduler();

        pixel_ = new pixel_t *[width_];
        for (int i = 0; i < width_; i++)
            pixel_[i] = new pixel_t[height_];
    }

    MultiPanel::~MultiPanel() {
        // TODO: Release memory panels_ and pixel_
        // TODO: Release scheduler
    }

    bool MultiPanel::map_panel(int x, int y, CFG *cfg) {
        Panel_t *ptr = new Panel_t;

        if (cfg == nullptr)
            throw Null_Pointer("CFG");

        ptr->cfg= cfg;
        ptr->x = x;
        ptr->y = y;
        ptr->panel = RGBMatrix::CreatePanel(cfg);

        lock_.lock();
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it) {
            if ((*it)->cfg == ptr->cfg) {
                delete ptr->panel;
                delete ptr;
                lock_.unlock();
                return false;
            }
        }

        scheduler_->add_protocol(cfg->get_protocol());
        panel_->push_back(ptr);
        lock_.unlock();
        return true;
    }

    void MultiPanel::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
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

    cord_t MultiPanel::get_size() {
        cord_t result;
        result.x = width_;
        result.y = height_;
        return result;
    }

    void MultiPanel::show() {
        lock_.lock();
        // TODO: Use scheduler
        // TODO: Convert pixel_ to panels_[x]->panel->set_pixel (implement runnables?)
        // TODO: Call show for every panel
        lock_.unlock();
    }


    void MultiPanel::set_brightness(uint8_t brightness) {
        lock_.lock();
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            (*it)->panel->set_brightness(brightness);
        lock_.unlock();
    }

    void MultiPanel::map_wavelength(uint8_t color, Color index, uint16_t value) {
        lock_.lock();
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            (*it)->panel->map_wavelength(color, index, value);
        lock_.unlock();
    }
}
