#ifndef MULTIPLEX_H
#define MULTIPLEX_H

#include <vector>

namespace rgb_matrix {
  class MultiplexMapper {
    public:
      MultiplexMapper(const char *name, int stretch_factor)
        : name_(name), panel_stretch_factor_(stretch_factor) {}

      // This method is const, but we sneakily remember the original size
      // of the panels so that we can more easily quantize things.
      // So technically, we're stateful, but let's pretend we're not changing
      // state. In the context this is used, it is never accessed in multiple
      // threads.
      virtual void EditColsRows(int *cols, int *rows) const {
        panel_rows_ = *rows;
        panel_cols_ = *cols;

        *rows /= panel_stretch_factor_;
        *cols *= panel_stretch_factor_;
      }

      virtual bool GetSizeMapping(int matrix_width, int matrix_height,
                                  int *visible_width, int *visible_height) const {
        // Matrix width has been altered. Alter it back.
        *visible_width = matrix_width / panel_stretch_factor_;
        *visible_height = matrix_height * panel_stretch_factor_;
        return true;
      }

      virtual const char *GetName() const { return name_; }

      // The MapVisibleToMatrix() as required by PanelMatrix here breaks it
      // down to the individual panel, so that derived classes only need to
      // implement MapSinglePanel().
      virtual void MapVisibleToMatrix(int matrix_width, int matrix_height,
                                      int visible_x, int visible_y,
                                      int *matrix_x, int *matrix_y) const {
        const int chained_panel  = visible_x / panel_cols_;
        const int parallel_panel = visible_y / panel_rows_;

        const int within_panel_x = visible_x % panel_cols_;
        const int within_panel_y = visible_y % panel_rows_;

        int new_x, new_y;
        MapSinglePanel(within_panel_x, within_panel_y, &new_x, &new_y);
        *matrix_x = chained_panel  * panel_stretch_factor_*panel_cols_ + new_x;
        *matrix_y = parallel_panel * panel_rows_/panel_stretch_factor_ + new_y;
      }

      // Map the coordinates for a single panel. This is to be overridden in
      // derived classes.
      // Input parameter is the visible position on the matrix, and this method
      // should return the internal multiplexed position.
      virtual void MapSinglePanel(int visible_x, int visible_y,
                                  int *matrix_x, int *matrix_y) const = 0;

    protected:
      const char *const name_;
      const int panel_stretch_factor_;

      mutable int panel_cols_;
      mutable int panel_rows_;
  };

  // Returns a vector of the registered Multiplex mappers.
  typedef std::vector<const MultiplexMapper*> MuxMapperList;
  const MuxMapperList &GetRegisteredMultiplexMappers();
}

#endif
