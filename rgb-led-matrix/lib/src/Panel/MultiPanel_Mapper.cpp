#include <chrono>
#include <assert.h>
#include "Panel/MultiPanel_Mapper.h"

namespace rgb_matrix {
    MultiPanel_Mapper::MultiPanel_Mapper() {
        assert(false);
    }

    MultiPanel_Mapper::MultiPanel_Mapper(int width, int height, int threads) : width_(width), height_(height), thread_count_(threads) {
        assert(threads > 0);
        shutdown_ = false;

        for (int i = 0; i < thread_count_; i++) {
            // TODO:
        }

        // TODO:

        for (int i = 0; i < width_; i++)
            pixel_[i] = new pixel_t[height_];
    }

    MultiPanel_Mapper::~MultiPanel_Mapper() {
        // TODO:
    }

    // TODO: Check for duplicates!
    void MultiPanel_Mapper::map_panel(int x, int y, Panel_Pixel_Mapper *panel) {
        Panel_t *ptr = new Panel_t;

        ptr->panel= panel;
        ptr->x = x;
        ptr->y = y;

        lock_.lock();
        panel_->push_back(ptr);
        lock_.unlock();
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
        for (int i = 0; i < panel_->size(); i++)
            panel_->at(i)->panel->set_brightness(brightness);
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