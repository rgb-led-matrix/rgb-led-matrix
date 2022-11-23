#include "Multiplex.h"

class ShiftRegisterMultiplex : public Multiplex {
    public:
        ShiftRegisterMultiplex(int rows);
    
        void SetRow(int row);
};
