#ifndef LEDPANEL_H
#define LEDPANEL_H

#include "Panel/Panel.h"
#include "framebuffer/Framebuffer.h"

namespace rgb_matrix {
  template <typename T> class LEDPanel : public Panel {
    public:
      LEDPanel(Framebuffer<T> *frame);
      virtual ~LEDPanel();

      virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
      virtual void show();
      virtual void set_brightness(uint8_t brightness);

    protected:
      Framebuffer<T> *const frame_;
  };
}
#endif