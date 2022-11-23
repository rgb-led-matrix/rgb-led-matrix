#include "Multiplex.h"

class DirectMultiplex : public Multiplex {
    public:
        DirectMultiplex(int rows);
    
        void SetRow(int row);
};
