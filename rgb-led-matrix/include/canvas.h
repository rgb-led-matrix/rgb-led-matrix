// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Copyright (C) 2014 Henner Zeller <h.zeller@acm.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://gnu.org/licenses/gpl-2.0.txt>

#ifndef CANVAS_H
#define CANVAS_H

#include <stdint.h>
#include <stddef.h>

namespace rgb_matrix {

  enum Canvas_ID {
    RP2040 = 0
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
  };

}
#endif
