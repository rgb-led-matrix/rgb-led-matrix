#ifndef FRAME_H
#define FRAME_H

#include <Panel/MultiPanel.h>
#include <IO/Protocol/Protocol.h>

namespace rgb_matrix {
    class Frame_Manager;

    class Frame : public Mapper {
        public:
            Frame(Single_Panel *panel, Protocol *protocol);
            Frame(MultiPanel *panel);

            bool isFree();

            void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
            cord_t get_size();
            void set_brightness(uint8_t brightness);
            void map_wavelength(uint8_t color, Color index, uint16_t value);
            
        protected:
            Frame();

            Single_Panel *single_;
            MultiPanel *multi_;
            Protocol *protocol_;
            bool isMulti_;
            bool isFree_;

            friend class Frame_Manager;
    };
}
#endif
