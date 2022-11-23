#ifndef MULTIPLEX_H
#define MULTIPLEX_H
    class Multiplex {
        public:
            virtual ~Multiplex();
            static Multiplex *getMultiplexer(int rows, int num);
            virtual void SetRow(int row) = 0;
            
        protected:
            Multiplex(int rows);
            
            int _rows;
            const int a = 1 << 0;
            const int b = 1 << 1;
            const int c = 1 << 2;
            const int d = 1 << 3;
            const int e = 1 << 4;
    };
#endif
