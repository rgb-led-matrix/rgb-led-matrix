#include "pin-pulser/PinPulser.h"

namespace rgb_matrix {
    std::mutex PinPulser::mutex_;
    bool PinPulser::isTriggered_ = false;
    bool PinPulser::isNotified_ = false;

    void PinPulser::trigger() {
        mutex_.lock();
        if (!isTriggered_) {
            isTriggered_ = true;
            isNotified_ = false;
        }
        mutex_.unlock();
    }

    void PinPulser::notify() {
        mutex_.lock();
        if (isTriggered_) {
            isTriggered_ = false;
            isNotified_ = true;
        }
        mutex_.unlock();
    }

    bool PinPulser::isTriggered() {
        bool result;
        mutex_.lock();
        result = isTriggered_;
        mutex_.unlock();
        return result;
    }

    bool PinPulser::isNotified() {
        bool result;
        mutex_.lock();
        result = isNotified_;
        mutex_.unlock();
        return result;
    }
}