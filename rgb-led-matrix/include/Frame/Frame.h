#ifndef FRAME_H
#define FRAME_H

#include <Panel/MultiPanel.h>
#include <IO/Protocol/Protocol.h>

namespace rgb_matrix {
    class Frame : public Drawer {
        public:
            Frame(Panel *panel, Protocol *protocol, Control *control);
            Frame(MultiPanel *panel, Control *control);

            bool isFree();

            void SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
            cord_t get_size();
            void set_brightness(uint8_t brightness);
            void map_wavelength(uint8_t color, Color index, uint16_t value);
            
        protected:
            Frame();

            Panel *single_;
            MultiPanel *multi_;
            Protocol *protocol_;
            Control *control_;
            bool isMulti_;
            bool isFree_;

            friend class Frame_Manager;
    };
}
#endif
