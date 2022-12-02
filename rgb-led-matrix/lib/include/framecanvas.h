#ifndef FRAMECANVAS_H
#define FRAMECANVAS_H

#include "canvas.h"
#include "framebuffer/framebuffer.h"

namespace rgb_matrix {
class FrameCanvas : public Canvas {
public:
  virtual ~FrameCanvas();

  void SetBrightness(uint8_t brightness);
  uint8_t brightness();

  void Serialize(const char **data, size_t *len, Canvas_ID *id) const;
  bool Deserialize(const char *data, size_t len, Canvas_ID id);

  virtual int width() const;
  virtual int height() const;
  virtual void SetPixel(int x, int y,
                        uint8_t red, uint8_t green, uint8_t blue);
  virtual void Clear();
  virtual void Fill(uint8_t red, uint8_t green, uint8_t blue);

  virtual void show();

private:
  FrameCanvas(Framebuffer *frame) : frame_(frame) {}
  Framebuffer *framebuffer() { return frame_; }

  Framebuffer *const frame_;
};
}
#endif