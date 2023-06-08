#include "mappers/MultiplexMapper.h"
#include "mappers/multiplex/StripeMultiplexMapper.h"

namespace rgb_matrix {
  static MuxMapperList *CreateMultiplexMapperList() {
    MuxMapperList *result = new MuxMapperList();

    // Here, register all multiplex mappers from above.
    result->push_back(new StripeMultiplexMapper());
    return result;
  }

  const MuxMapperList &GetRegisteredMultiplexMappers() {
    static const MuxMapperList *all_mappers = CreateMultiplexMapperList();
    return *all_mappers;
  }
}
