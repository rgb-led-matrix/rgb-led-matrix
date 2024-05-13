#ifndef CONTROL_H
#define CONTROL_H

#include <IO/Node/Node.h>

namespace rgb_matrix {
    class Control {
        public:
            Control(Node *node);
            virtual ~Control() {}
        
            enum class Commands {
                Trigger,
                Reset,
                Acknowledge
            };

            // For Scheduler
            virtual void signal(Commands command) = 0;

        protected:
            Control();

            Node *node_;
    };
}
#endif
