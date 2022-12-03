#ifndef FRAMECANVAS_H
#define FRAMECANVAS_H

#include "canvas.h"
#include "framebuffer/framebuffer.h"

namespace rgb_matrix {
  template <typename T> class FrameCanvas : public Canvas {
    public:
      FrameCanvas(Framebuffer<T> *frame) : frame_(frame) {}
      virtual ~FrameCanvas();

      virtual int width() const;
      virtual int height() const;

      virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
      virtual void Clear();
      virtual void Fill(uint8_t red, uint8_t green, uint8_t blue);

      void Serialize(const char **data, size_t *len, Canvas_ID *id);
      bool Deserialize(const char *data, size_t len, Canvas_ID id);

      virtual void show();

    protected:
      Framebuffer<T> *const frame_;
  };
}
#endif