#include <cmath>
#include "RP2040/RP2040.h"

namespace rgb_matrix {
RP2040::RP2040(int rows, int columns, int parallel, PixelDesignatorMap **mapper) {
  bitplane_buffer_ = new gpio_bits_t[double_rows_ * columns_ * 6];
  buffer_size_ = (double_rows_ * columns_ * 6 * sizeof(gpio_bits_t));

  // If we're the first Framebuffer created, the shared PixelMapper is
  // still NULL, so create one.
  // The first PixelMapper represents the physical layout of a standard matrix
  // with the specific knowledge of the framebuffer, setting up PixelDesignators
  // in a way that they are useful for this Framebuffer.
  //
  // Newly created PixelMappers then can just re-arrange PixelDesignators
  // from the parent PixelMapper opaquely without having to know the details.
  if (*shared_mapper_ == NULL) {
    // Gather all the bits for given color for fast Fill()s and use the right
    // bits according to the led sequence
    const struct HardwareMapping &h = *hardware_mapping_;
    gpio_bits_t r = h.p0_r1 | h.p0_r2 | h.p1_r1 | h.p1_r2 | h.p2_r1 | h.p2_r2 | h.p3_r1 | h.p3_r2 | h.p4_r1 | h.p4_r2 | h.p5_r1 | h.p5_r2;
    gpio_bits_t g = h.p0_g1 | h.p0_g2 | h.p1_g1 | h.p1_g2 | h.p2_g1 | h.p2_g2 | h.p3_g1 | h.p3_g2 | h.p4_g1 | h.p4_g2 | h.p5_g1 | h.p5_g2;
    gpio_bits_t b = h.p0_b1 | h.p0_b2 | h.p1_b1 | h.p1_b2 | h.p2_b1 | h.p2_b2 | h.p3_b1 | h.p3_b2 | h.p4_b1 | h.p4_b2 | h.p5_b1 | h.p5_b2;

    *shared_mapper_ = new PixelDesignatorMap(columns_, height_);
    for (int y = 0; y < height_; ++y) {
      for (int x = 0; x < columns_; ++x) {
        InitDefaultDesignator(x, y, (*shared_mapper_)->get(x, y));
      }
    }
  }
}

RP2040::~RP2040() {
  delete [] bitplane_buffer_;
}

inline gpio_bits_t *RP2040::ValueAt(int double_row, int column, int bit) {
  return &bitplane_buffer_[ double_row * (columns_ * kBitPlanes)
                            + bit * columns_
                            + column ];
}

// Do CIE1931 luminance correction and scale to output bitplanes
uint16_t RP2040::luminance_cie1931(uint8_t c, uint8_t brightness) {
  float out_factor = ((1 << pwm_bits_) - 1);
  float v = (float) c * brightness / 255.0;
  return roundf(out_factor * ((v <= 8) ? v / 902.3 : pow((v + 16) / 116.0, 3)));
}

RP2040::ColorLookup *RP2040::CreateLuminanceCIE1931LookupTable() {
  ColorLookup *for_brightness = new ColorLookup[100];
  for (int c = 0; c < 256; ++c)
    for (int b = 0; b < 100; ++b)
      for_brightness[b].color[c] = luminance_cie1931(c, b + 1);

  return for_brightness;
}

bool RP2040::SetPWMBits(uint8_t value) {
  if (value < 1 || value > 16)
    return false;
  pwm_bits_ = value;
  luminance_lookup = CreateLuminanceCIE1931LookupTable();
  return true;
}

inline uint16_t RP2040::CIEMapColor(uint8_t brightness, uint8_t c) {
  return luminance_lookup[brightness - 1].color[c];
}

// Non luminance correction. TODO: consider getting rid of this.
static inline uint16_t DirectMapColor(uint8_t brightness, uint8_t c) {
  // simple scale down the color value
  c = c * brightness / 100;

  // shift to be left aligned with top-most bits.
  constexpr int shift = Framebuffer::kBitPlanes - 8;
  return (shift > 0) ? (c << shift) : (c >> -shift);
}

// TODO: Fix this
inline void RP2040::MapColors(
  uint8_t r, uint8_t g, uint8_t b,
  uint16_t *red, uint16_t *green, uint16_t *blue) {

  if (do_luminance_correct_) {
    *red   = CIEMapColor(brightness_, r);
    *green = CIEMapColor(brightness_, g);
    *blue  = CIEMapColor(brightness_, b);
  } else {
    *red   = DirectMapColor(brightness_, r);
    *green = DirectMapColor(brightness_, g);
    *blue  = DirectMapColor(brightness_, b);
  }
}

void RP2040::InitDefaultDesignator(int x, int y, PixelDesignator *d) {
  // TODO
}

void RP2040::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
  // TODO
}

void Framebuffer::DumpToMatrix(GPIO *io, int pwm_low_bit) {
  // TODO
}
}  // namespace rgb_matrix
