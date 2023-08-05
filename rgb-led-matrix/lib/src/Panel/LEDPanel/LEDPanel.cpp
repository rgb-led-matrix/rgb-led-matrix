#include "Panel/LEDPanel/LEDPanel.h"
#include "framebuffer/external/external.h"

namespace rgb_matrix {

  template <typename T> LEDPanel<T>::~LEDPanel() { 
    delete frame_; 
  }

  template <typename T> void LEDPanel<T>::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    frame_->SetPixel(x, y, red, green, blue);
  }

  template <typename T> void LEDPanel<T>::show() {
    frame_->show();
  }

  template class LEDPanel<RGB48>;
}