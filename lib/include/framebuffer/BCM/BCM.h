#ifndef BCM_H
#define BCM_H

#include "framebuffer.h"

namespace rgb_matrix {
class BCM : public Framebuffer {
public:
  BCM(int rows, int columns, int parallel, PixelDesignatorMap **mapper);
  ~BCM();

  void DumpToMatrix(GPIO *io, int pwm_bits_to_show);
  void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);

protected:
  inline gpio_bits_t *ValueAt(int double_row, int column, int bit);
  inline void  MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue);

private:
  void InitDefaultDesignator(int x, int y, PixelDesignator *designator);
};
}  // namespace rgb_matrix
#endif // RPI_RGBMATRIX_FRAMEBUFFER_INTERNAL_H
