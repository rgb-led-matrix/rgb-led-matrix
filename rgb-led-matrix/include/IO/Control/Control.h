#ifndef CONTROL_H
#define CONTROL_H

#include <IO/Node/Node.h>

namespace rgb_matrix {
    class Control {
        public:
            Control(Node *node);
            virtual ~Control() {}

            // For Scheduler
            virtual void signal() = 0;

        protected:
            Control();

            Node *node_;
    };
}
#endif
