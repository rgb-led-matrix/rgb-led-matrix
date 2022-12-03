#ifndef CANVAS_H
#define CANVAS_H

#include <stdint.h>
#include <stddef.h>

namespace rgb_matrix {

  enum Canvas_ID {
    RP2040_ID = 0
  };

  class Canvas {
    public:
      virtual ~Canvas() {}
      virtual int width() const = 0;  // Pixels available in x direction.
      virtual int height() const = 0; // Pixels available in y direction.

      virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) = 0;
      virtual void Clear() = 0;
      virtual void Fill(uint8_t red, uint8_t green, uint8_t blue) = 0;

      void Serialize(const char **data, size_t *len, Canvas_ID *id) const;
      bool Deserialize(const char *data, size_t len, Canvas_ID id);

    protected:
      friend class RGBMatrix;

      virtual void show() = 0;
  };

}
#endif
