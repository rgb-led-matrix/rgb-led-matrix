#include <chrono>
#include <Panel/MultiPanel_Mapper.h>
#include <Exception/String_Exception.h>

// TODO: Make this single threaded for now

namespace rgb_matrix {
    // Do not use this!
    MultiPanel_Mapper::MultiPanel_Mapper() {
        // Do nothing
    }

    MultiPanel_Mapper::MultiPanel_Mapper(int width, int height, int threads) : width_(width), height_(height), thread_count_(threads) {
        shutdown_ = false;

        throw String_Exception("Not finished");

        if (thread_count_ <= 0)
            thread_count_ = 1;

        for (int i = 0; i < thread_count_; i++) {
            // TODO:
        }

        // TODO:

        pixel_ = new pixel_t *[width_];
        for (int i = 0; i < width_; i++)
            pixel_[i] = new pixel_t[height_];
    }

    MultiPanel_Mapper::~MultiPanel_Mapper() {
        // TODO:
    }


    bool MultiPanel_Mapper::map_panel(int x, int y, Panel_Pixel_Mapper *panel) {
        return map_panel(x, y, panel);
    }

    // TODO: Check for duplicates!
    bool MultiPanel_Mapper::map_panel(int x, int y, Panel *panel) {
        Panel_t *ptr = new Panel_t;

        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it) {
            if ((*it)->panel->get_protocol() == panel->get_protocol())
                return false;
        }

        ptr->panel= panel;
        ptr->x = x;
        ptr->y = y;

        lock_.lock();
        panel_->push_back(ptr);
        lock_.unlock();

        return true;
    }

    void MultiPanel_Mapper::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
        cord_t cord;
        pixel_t pixel;

        cord.x = x;
        cord.y = y;
        pixel.red = red;
        pixel.green = green;
        pixel.blue = blue;

        SetPixel(cord, pixel);
    }

    void MultiPanel_Mapper::SetPixel(cord_t cord, pixel_t pixel) {
        lock_.lock();
        pixel_[cord.x][cord.y] = pixel;
        lock_.unlock();
    }

    cord_t MultiPanel_Mapper::get_size() {
        cord_t result;

        result.x = width_;
        result.y = height_;

        return result;
    }

    void MultiPanel_Mapper::show() {
        bool done = false;

        lock_.lock();
        // TODO: Submit SetPixel jobs

        while (!done) {
            queue_lock_.lock();

            if (!queue_.empty())
                done = true;

            queue_lock_.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        // TODO: Submit show jobs

        while (!done) {
            queue_lock_.lock();

            if (!queue_.empty())
                done = true;

            queue_lock_.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        lock_.unlock();
    }

    void MultiPanel_Mapper::set_brightness(uint8_t brightness) {
        lock_.lock();
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            (*it)->panel->set_brightness(brightness);
        lock_.unlock();
    }

    void MultiPanel_Mapper::map_wavelength(uint8_t color, Color index, uint16_t value) {
        lock_.lock();
        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it)
            (*it)->panel->map_wavelength(color, index, value);
        lock_.unlock();
    }

    void MultiPanel_Mapper::thread(void *args) {
        MultiPanel_Mapper *ptr = (MultiPanel_Mapper *) args;

        while (!ptr->shutdown_) {
            Panel_t *panel = nullptr;

            ptr->queue_lock_.lock();
            
            if (!ptr->queue_.empty()) {
                panel = ptr->queue_.front();
                ptr->queue_.pop();
            }
            
            ptr->queue_lock_.unlock();

            if (panel != nullptr)
                ptr->task(panel);
        }
    }

    void MultiPanel_Mapper::task(Panel_t *panel) {
        // TODO:
        //  Support two operations (SetPixel and show)
    }
}  // namespace rgb_matrix