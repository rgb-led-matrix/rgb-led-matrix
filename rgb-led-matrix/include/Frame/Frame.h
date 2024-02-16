#ifndef FRAME_H
#define FRAME_H

#include <MultiPanel.h>
#include <IO/Protocol/Protocol.h>

namespace rgb_matrix {
    class Frame : public Mapper {
        public:
            Frame(Single_Panel *panel, Protocol *protocol);
            Frame(MultiPanel *panel);

            void show();
            
        protected:
            Frame();

            Panel *panel;
            Protocol *panel;
            bool isMulti;
    };
}
#endif
