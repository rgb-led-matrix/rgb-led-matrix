#ifndef EVENT_H
#define EVENT_H

namespace rgb_matrix {
    // Pure interface for control
    class Event {
        public:
            virtual void show() = 0;
    };
}
#endif
