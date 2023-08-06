#ifndef PANEL_PIXEL_MAPPER_H
#define PANEL_PIXEL_MAPPER_H

#include <stdint.h>
#include "Panel/Panel.h"
#include "CFG/types.h"

namespace rgb_matrix {
  class Panel_Pixel_Mapper {
    public:
      Panel_Pixel_Mapper(Panel *panel);
      virtual ~Panel_Pixel_Mapper();

      void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
      void SetPixel(cord_t cord, pixel_t pixel);
      void show();

      // Applies to SetPixel calls after set
      void set_brightness(uint8_t brightness);

    protected:
      Panel_Pixel_Mapper();

      void map();
      virtual cord_t map_location(cord_t cur_cord) = 0;
      virtual void map_color(cord_t cord, Color_Order order) = 0;

      Panel *panel_;
      cord_t **locations_;
      Color_Order **orders_;
  };
}
#endif
