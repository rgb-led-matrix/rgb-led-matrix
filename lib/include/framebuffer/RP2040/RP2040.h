#ifndef RP2040_H
#define RP2040_H

#include "framebuffer.h"

namespace rgb_matrix {
class RP2040 : public Framebuffer {
public:
  RP2040(int rows, int columns, int parallel, PixelDesignatorMap **mapper);
  ~RP2040();

  bool SetPWMBits(uint8_t value);
  void DumpToMatrix(GPIO *io, int pwm_bits_to_show);
  void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);

protected:
  inline gpio_bits_t *ValueAt(int double_row, int column, int bit);
  inline void  MapColors(uint8_t r, uint8_t g, uint8_t b, uint16_t *red, uint16_t *green, uint16_t *blue);

private:
  void InitDefaultDesignator(int x, int y, PixelDesignator *designator);

  struct ColorLookup {
    uint16_t color[256];
  };

  ColorLookup *luminance_lookup;

  ColorLookup *CreateLuminanceCIE1931LookupTable();
  uint16_t luminance_cie1931(uint8_t c, uint8_t brightness);
  inline uint16_t CIEMapColor(uint8_t brightness, uint8_t c);
};
}  // namespace rgb_matrix
#endif // RPI_RGBMATRIX_FRAMEBUFFER_INTERNAL_H
