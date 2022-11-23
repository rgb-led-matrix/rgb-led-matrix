#include "Multiplex.h"

class ABCShiftRegisterMultiplex : public Multiplex {
    public:
        ABCShiftRegisterMultiplex(int rows);
    
        void SetRow(int row);
};
