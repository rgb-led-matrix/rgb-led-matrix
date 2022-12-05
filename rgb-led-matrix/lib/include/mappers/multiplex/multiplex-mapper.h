#ifndef MULTIPLEX_H
#define MULTIPLEX_H

#include <vector>
#include "mappers/pixel/HUB75/pixel-mapper.h"

namespace rgb_matrix {
  class MultiplexMapper : public PixelMapper {
    public:
      // Function that edits the original rows and columns of the panels
      // provided by the user to the actual underlying mapping. This is called
      // before we do the actual set-up of the GPIO mapping as this influences
      // the hardware interface.
      // This is so that the user can provide the rows/columns they see.
      virtual void EditColsRows(int *cols, int *rows) const = 0;
  };

  // Returns a vector of the registered Multiplex mappers.
  typedef std::vector<const MultiplexMapper*> MuxMapperList;
  const MuxMapperList &GetRegisteredMultiplexMappers();
}

#endif
