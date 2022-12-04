#include "framecanvas.h"

namespace rgb_matrix {

  template <typename T> FrameCanvas<T>::~FrameCanvas() { 
    delete frame_; 
  }

  template <typename T> int FrameCanvas<T>::width() const { 
    return frame_->width(); 
  }

  template <typename T> int FrameCanvas<T>::height() const { 
    return frame_->height(); 
  }

  template <typename T> void FrameCanvas<T>::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    frame_->SetPixel(x, y, red, green, blue);
  }

  template <typename T> void FrameCanvas<T>::Clear() { 
    return Fill(0, 0, 0);
  }

  template <typename T> void FrameCanvas<T>::Fill(uint8_t red, uint8_t green, uint8_t blue) {
    for (int x = 0; x < frame_->width(); ++x) {
      for (int y = 0; y < frame_->height(); ++y) {
        frame_->SetPixel(x, y, red, green, blue);
      }
    }
  }

  template <typename T> void FrameCanvas<T>::Serialize(const char **data, size_t *len, Canvas_ID *id) {
    frame_->Serialize(data, len, id);
  }

  template <typename T> bool FrameCanvas<T>::Deserialize(const char *data, size_t len, Canvas_ID id) {
    return frame_->Deserialize(data, len, id);
  }

  template <typename T> void FrameCanvas<T>::show() {
    frame_->DumpToMatrix();
  }

  template class FrameCanvas<PixelDesignator>;
  template class FrameCanvas<PixelDesignator_HUB75>;

}