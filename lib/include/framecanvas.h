#ifndef FRAMECANVAS_H
#define FRAMECANVAS_H

#include "canvas.h"
#include "framebuffer.h"

namespace rgb_matrix {
class FrameCanvas : public Canvas {
public:
  // Set PWM bits used for this Frame.
  // Simple comic-colors, 1 might be sufficient (111 RGB, i.e. 8 colors).
  // Lower require less CPU.
  // Returns boolean to signify if value was within range.
  bool SetPWMBits(uint8_t value);
  uint8_t pwmbits();

  // Map brightness of output linearly to input with CIE1931 profile.
  void set_luminance_correct(bool on);
  bool luminance_correct() const;

  void SetBrightness(uint8_t brightness);
  uint8_t brightness();

  //-- Serialize()/Deserialize() are fast ways to store and re-create a canvas.

  // Provides a pointer to a buffer of the internal representation to
  // be copied out for later Deserialize().
  //
  // Returns a "data" pointer and the data "len" in the given out-paramters;
  // the content can be copied from there by the caller.
  //
  // Note, the content is not simply RGB, it is the opaque and platform
  // specific representation which allows to make deserialization very fast.
  // It is also bigger than just RGB; if you want to store it somewhere,
  // using compression is a good idea.
  void Serialize(const char **data, size_t *len, Canvas_ID *id) const;

  // Load data previously stored with Serialize(). Needs to be restored into
  // a FrameCanvas with exactly the same settings (rows, chain, transformer,...)
  // as serialized.
  // Returns 'false' if size is unexpected.
  // This method should only be called if FrameCanvas is off-screen.
  bool Deserialize(const char *data, size_t len, Canvas_ID id);

  // -- Canvas interface.
  virtual int width() const;
  virtual int height() const;
  virtual void SetPixel(int x, int y,
                        uint8_t red, uint8_t green, uint8_t blue);
  virtual void Clear();
  virtual void Fill(uint8_t red, uint8_t green, uint8_t blue);

private:
  friend class RGBMatrix;

  FrameCanvas(Framebuffer *frame) : frame_(frame){}
  virtual ~FrameCanvas();   // Any FrameCanvas is owned by RGBMatrix.
  Framebuffer *framebuffer() { return frame_; }

  Framebuffer *const frame_;
};
}
#endif