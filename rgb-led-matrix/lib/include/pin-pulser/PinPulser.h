#ifndef PIN_PULSER_H
#define PIN_PULSER_H

#include <mutex>

namespace rgb_matrix {
    class PinPulser {
        public:
            static void trigger();
            static void notify();
            static bool isTriggered();
            static bool isNotified();

        private:
            static std::mutex mutex_;
            static bool isTriggered_;
            static bool isNotified_;
    };
}
#endif