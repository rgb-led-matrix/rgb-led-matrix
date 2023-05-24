#ifndef STRIPEMULTIPLEXMAPPER_H
#define STRIPEMULTIPLEXMAPPER_H

#include "mappers/MultiplexMapper.h"

namespace rgb_matrix {
    class StripeMultiplexMapper : public MultiplexMapper {
        public:
            StripeMultiplexMapper() : MultiplexMapper("Stripe", 2) {}

            void MapSinglePanel(int x, int y, int *matrix_x, int *matrix_y) const {
                const bool is_top_stripe = (y % (panel_rows_/2)) < panel_rows_/4;
                *matrix_x = is_top_stripe ? x + panel_cols_ : x;
                *matrix_y = ((y / (panel_rows_/2)) * (panel_rows_/4)
                            + y % (panel_rows_/4));
            }
  };
}

#endif