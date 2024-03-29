#ifndef COM_CONTROL_H
#define COM_CONTROL_H

#include <IO/Control/Control.h>

namespace rgb_matrix {
    class COM_Control : public Control {
        public:
            COM_Control(Node *node);
        
            void signal();

        protected:
            COM_Control();
    };
}
#endif
