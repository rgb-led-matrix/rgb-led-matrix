#include "framecanvas.h"

namespace rgb_matrix {
// FrameCanvas implementation of Canvas
FrameCanvas::~FrameCanvas() { delete frame_; }
int FrameCanvas::width() const { return frame_->width(); }
int FrameCanvas::height() const { return frame_->height(); }
void FrameCanvas::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
  frame_->SetPixel(x, y, red, green, blue);
}
void FrameCanvas::Clear() { return Fill(0, 0, 0); }

void FrameCanvas::Fill(uint8_t red, uint8_t green, uint8_t blue) {
  for (int x = 0; x < frame_->width(); ++x) {
    for (int y = 0; y < frame_->height(); ++y) {
      frame_->SetPixel(x, y, red, green, blue);
    }
  }
}
bool FrameCanvas::SetPWMBits(uint8_t value) { return frame_->SetPWMBits(value); }
uint8_t FrameCanvas::pwmbits() { return frame_->pwmbits(); }

// Map brightness of output linearly to input with CIE1931 profile.
void FrameCanvas::set_luminance_correct(bool on) { frame_->set_luminance_correct(on); }
bool FrameCanvas::luminance_correct() const { return frame_->luminance_correct(); }

void FrameCanvas::SetBrightness(uint8_t brightness) { frame_->SetBrightness(brightness); }
uint8_t FrameCanvas::brightness() { return frame_->brightness(); }

void FrameCanvas::Serialize(const char **data, size_t *len, Canvas_ID *id) const {
  frame_->Serialize(data, len, id);
}
bool FrameCanvas::Deserialize(const char *data, size_t len, Canvas_ID id) {
  return frame_->Deserialize(data, len, id);
}
}