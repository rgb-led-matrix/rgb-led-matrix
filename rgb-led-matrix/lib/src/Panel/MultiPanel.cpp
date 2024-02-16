#include <chrono>
#include <Panel/MultiPanel.h>
#include <Exception/Illegal.h>

// TODO: Make this single threaded for now

// WARNING: Does not check underlying protocols for multiple copys of the same protocol in same frame.
//  This is bad. Sharing nodes and protocols within a frame is undefined.
//  Protocols may reuse nodes but must be in different frames.
//  Frames may reuse protocols, but not within the same frame.
//  We are stuck with this because of templating which comes from variable RGB notions.

namespace rgb_matrix {
    // Do not use this!
    MultiPanel::MultiPanel() {
        throw Illegal("MultiPanel Panel");
    }

    MultiPanel::MultiPanel(int width, int height, int threads) : width_(width), height_(height), thread_count_(threads) {
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

    MultiPanel::~MultiPanel() {
        // TODO:
    }

    // TODO: Check for duplicates!
    bool MultiPanel::map_panel(int x, int y, Panel *panel) {
        Panel_t *ptr = new Panel_t;

        for (std::list<Panel_t *>::iterator it = panel_->begin(); it != panel_->end(); ++it) {
            if ((*it)->panel == panel)
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

    void MultiPanel::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
        cord_t cord;
        pixel_t pixel;

        cord.x = x;
        cord.y = y;
        pixel.red = red;
        pixel.green = green;
        pixel.blue = blue;

        SetPixel(cord, pixel);
    }

    void MultiPanel::SetPixel(cord_t cord, pixel_t pixel) {
        lock_.lock();
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

    void MultiPanel::thread(void *args) {
        MultiPanel *ptr = (MultiPanel *) args;

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

    void MultiPanel::task(Panel_t *panel) {
        // TODO:
        //  Support two operations (SetPixel and show)
    }
}